// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"

void AWeaponItem::Use(APlayerCharacter* PlayerCharacter)
{
	if (!bIsEquipped)
	{
		PlayerCharacter->GetEquipmentComponent()->AddItem(Data, 0);
		bIsEquipped = true;
	} else
	{
		PlayerCharacter->GetInventoryComponent()->AddItem(Data, PlayerCharacter->GetInventoryComponent()->GetFirstEmptySlotPosition());
		bIsEquipped = false;
	}
}
