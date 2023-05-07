// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributesComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCharacterAttributesComponent::UCharacterAttributesComponent()
{
	// Health
	MaxHealth = 50.0f;
	Health = MaxHealth;

	// Stamina
	MaxStamina = 50.0f;
	Stamina = MaxStamina;
}

void UCharacterAttributesComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCharacterAttributesComponent, Health);
	DOREPLIFETIME(UCharacterAttributesComponent, Stamina);
}

// Called when the game starts
void UCharacterAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Updates health on the server
void UCharacterAttributesComponent::ServerUpdateHealth_Implementation(float HealthDelta)
{
	UpdateHealth(HealthDelta);
}

// Changes HUD after server health replication
void UCharacterAttributesComponent::OnRepHealth()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetHealth(Health, MaxHealth);
	}
}

// Set Player's HUD
void UCharacterAttributesComponent::SetUpHUD(UPlayerHUD* HUD)
{
	PlayerHUD = HUD;
	PlayerHUD->SetHealth(Health, MaxHealth);
	PlayerHUD->SetStamina(Stamina, MaxStamina);
}

// Calls server to update health or updates health locally on the server
void UCharacterAttributesComponent::UpdateHealth(float HealthDelta)
{
	if (!GetOwner()->HasAuthority())
	{
		ServerUpdateHealth(HealthDelta);
	}
	else
	{
		Health = FMath::Clamp(Health + HealthDelta, 0.f, MaxHealth);
		OnRepHealth();
	}
}

// Calls server to update stamina or updates stamina locally on the server
void UCharacterAttributesComponent::UpdateStamina(float StaminaDelta)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateStamina %f"), StaminaDelta);

	Stamina = FMath::Clamp(Stamina + StaminaDelta, 0.f, MaxStamina);

	if (PlayerHUD)
	{
		PlayerHUD->SetStamina(Stamina, MaxStamina);
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
