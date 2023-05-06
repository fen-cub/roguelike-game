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

	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRepHealth)
	float Health;

	UFUNCTION(Server, Reliable)
	void ServerUpdateHealth(float HealthDelta);
	
	UFUNCTION()
	void OnRepHealth();

	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(Replicated)
	float Stamina;

public:
	void SetUpHUD(class UPlayerHUD* HUD);

	void UpdateHealth(float HealthDelta);

	void UpdateStamina(float StaminaDelta);

	float GetHealth() const;

	float GetStamina() const;
};
