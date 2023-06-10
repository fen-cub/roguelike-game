// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorItem.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"

void AArmorItem::Use(APlayerCharacter* PlayerCharacter)
{
	if (!bIsEquipped)
	{
		PlayerCharacter->GetEquipmentComponent()->AddItem(Data, 1);
		bIsEquipped = true;
	} else
	{
		PlayerCharacter->GetInventoryComponent()->AddItem(Data, PlayerCharacter->GetInventoryComponent()->GetFirstEmptySlotPosition());
		bIsEquipped = false;
	}
}
