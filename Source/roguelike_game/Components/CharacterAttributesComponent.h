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
	class IDisplayInterface* Display;

	UPROPERTY(Replicated)
	float DamageProtectionPercent;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(Replicated)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound | Config ")
	class USoundBase* DamageSound;
	
	UPROPERTY(EditAnywhere)
	float MaxStamina;

	UPROPERTY(ReplicatedUsing = OnRepUpdateStamina)
	float Stamina;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	// Called to update Health on the server
	UFUNCTION(Server, Unreliable)
	void ServerUpdateHealth(float HealthDelta);

	// Called back from server when health updated
	UFUNCTION(NetMulticast, Unreliable)
	void OnRepUpdateHealth(float HealthDelta);

	// Called to update Stamina on the server
	UFUNCTION(Server, Unreliable)
	void ServerUpdateStamina(float StaminaDelta);

	// Called back from server when stamina updated
	UFUNCTION()
	void OnRepUpdateStamina();

	// Called to set damage on the server
	UFUNCTION(Server, Unreliable)
	void ServerSetDamageProtection(float NewDamageProtection);

	// Called to damage character on server
	UFUNCTION(Server, Reliable)
	void ServerDamageCharacter(float Damage);

public:
	// Called when HUD created
	void SetUpDisplay(class IDisplayInterface* NewDisplay);

	// Called to update health
	void UpdateHealth(float HealthDelta);

	// Called to update stamina
	void UpdateStamina(float StaminaDelta);

	// Called to update damage protection
	void SetDamageProtection(float NewDamageProtectionPercent);

	// Call to damage character
	void DamageCharacter(float Damage);

	// Returns current health
	float GetHealth() const;

	// Return current stamina
	float GetStamina() const;
};
