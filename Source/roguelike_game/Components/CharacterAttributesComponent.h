// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "roguelike_game/Widgets/PlayerHUD.h"
#include "CharacterAttributesComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUELIKE_GAME_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAttributesComponent();

protected:
	// Need to be set at the begining of the game
	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRepUpdateHealth)
	float Health;

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(ReplicatedUsing = OnRepUpdateStamina)
	float Stamina;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Called to update Health on the server
	UFUNCTION(Server, Reliable)
	void ServerUpdateHealth(float HealthDelta);

	// Called back from server when health updated
	UFUNCTION()
	void OnRepUpdateHealth();

	// Called to update Stamina on the server
	UFUNCTION(Server, Reliable)
	void ServerUpdateStamina(float StaminaDelta);

	// Called back from server when stamina updated
	UFUNCTION()
	void OnRepUpdateStamina();

public:
	// Called when HUD created
	void SetUpHUD(class UPlayerHUD* HUD);

	// Called to update health
	void UpdateHealth(float HealthDelta);

	// Called to update stamina
	void UpdateStamina(float StaminaDelta);

	// Returns current health
	float GetHealth() const;

	// Return current stamina
	float GetStamina() const;
};
