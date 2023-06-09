// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesRecoveryItem.h"

#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/CharacterAttributesComponent.h"

AAttributesRecoveryItem::AAttributesRecoveryItem()
{
	HealthRecoveryValue = 10.0f;
	StaminaRecoveryValue = 10.0f;
}

void AAttributesRecoveryItem::Use(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->GetAttributesComponent()->UpdateHealth(HealthRecoveryValue);
	PlayerCharacter->GetAttributesComponent()->UpdateStamina(StaminaRecoveryValue);
}
