// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesRecoveryItem.h"

#include "Kismet/GameplayStatics.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/CharacterAttributesComponent.h"
#include "roguelike_game/Components/ItemStorageComponent.h"

AAttributesRecoveryItem::AAttributesRecoveryItem()
{
	HealthRecoveryValue = 10.0f;
	StaminaRecoveryValue = 10.0f;
}

void AAttributesRecoveryItem::Use(APlayerCharacter* PlayerCharacter, int64 InventoryPosition)
{
	PlayerCharacter->GetAttributesComponent()->UpdateHealth(HealthRecoveryValue);
	PlayerCharacter->GetAttributesComponent()->UpdateStamina(StaminaRecoveryValue);
	PlayerCharacter->GetInventoryComponent()->RemoveItem(InventoryPosition);
	UGameplayStatics::SpawnSoundAtLocation(PlayerCharacter, UseSound, PlayerCharacter->GetActorLocation());
}
