// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	SetIsReplicated(true);

	// Health
	MaxHealth = 50.0f;
	Health = MaxHealth;

	// Stamina
	MaxStamina = 50.0f;
	Stamina = MaxStamina;

	// DamageProtectionPercent
	DamageProtectionPercent = 0.0f;
}

// Called when the game starts
void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UCharacterAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterAttributesComponent, Health);
	DOREPLIFETIME(UCharacterAttributesComponent, Stamina);
	DOREPLIFETIME(UCharacterAttributesComponent, DamageProtectionPercent);
}

// Updates health on the server
void UCharacterAttributesComponent::ServerUpdateHealth_Implementation(float HealthDelta)
{
	OnRepUpdateHealth(HealthDelta);
}

void UCharacterAttributesComponent::OnRepUpdateHealth_Implementation(float HealthDelta)
{
	Health = FMath::Clamp(Health + HealthDelta, 0.f, MaxHealth);

	if (HealthDelta < 0.0f && !FMath::IsNearlyZero(Health))
	{
		UGameplayStatics::SpawnSoundAtLocation(this, DamageSound, GetOwner()->GetActorLocation());
	}

	if (Display)
	{
		Display->SetHealth(Health, MaxHealth);
	}
}

void UCharacterAttributesComponent::ServerUpdateStamina_Implementation(float StaminaDelta)
{
	Stamina = FMath::Clamp(Stamina + StaminaDelta, 0.f, MaxStamina);
	OnRepUpdateStamina();
}

void UCharacterAttributesComponent::OnRepUpdateStamina()
{
	if (Display)
	{
		Display->SetStamina(Stamina, MaxStamina);
	}
}

void UCharacterAttributesComponent::ServerSetDamageProtection_Implementation(float NewDamageProtection)
{
	DamageProtectionPercent = NewDamageProtection;
}

void UCharacterAttributesComponent::ServerDamageCharacter_Implementation(float Damage)
{
	UpdateHealth(-Damage * (100 - DamageProtectionPercent) / 100);
}

// Set Player's HUD
void UCharacterAttributesComponent::SetUpDisplay(IDisplayInterface* NewDisplay)
{
	Display = NewDisplay;
	Display->SetHealth(Health, MaxHealth);
	Display->SetStamina(Stamina, MaxStamina);
}

// Calls server to update health or updates health locally on the server
void UCharacterAttributesComponent::UpdateHealth(float HealthDelta)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerUpdateHealth(HealthDelta);
	}
}

// Calls server to update stamina or updates stamina locally on the server
void UCharacterAttributesComponent::UpdateStamina(float StaminaDelta)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerUpdateStamina(StaminaDelta);
	}
}

void UCharacterAttributesComponent::SetDamageProtection(float NewDamageProtection)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerSetDamageProtection(NewDamageProtection);
	}
}

void UCharacterAttributesComponent::DamageCharacter(float Damage)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerDamageCharacter(Damage);
	}
}


// Returns current health
float UCharacterAttributesComponent::GetHealth() const
{
	return Health;
}

// Returns current stamina
float UCharacterAttributesComponent::GetStamina() const
{
	return Stamina;
}
