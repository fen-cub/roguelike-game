// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../HUD/PlayerHUD.h"
#include "CharacterAttributesComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUELIKE_GAME_API UCharacterAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterAttributesComponent();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	// Need to be set at the begining of the game
	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
	float MaxHealth;
	
	UPROPERTY(ReplicatedUsing = OnRepHealth)
	float Health;

	// Called to update Health on the server
	UFUNCTION(Server, Reliable)
	void ServerUpdateHealth(float HealthDelta);

	// Called back from server when health updated
	UFUNCTION()
	void OnRepHealth();
	
	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(Replicated)
	float Stamina;

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
