// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponItem.h"

#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"

void AWeaponItem::Use(APlayerCharacter* PlayerCharacter, int64 InventoryPosition)
{
	if (!Data.bIsEquipped)
	{
		if (PlayerCharacter->GetEquipmentComponent()->GetItem(0).IsEmpty())
		{
			Data.bIsEquipped = true;
			PlayerCharacter->GetEquipmentComponent()->AddItem(Data, 0);
			PlayerCharacter->GetInventoryComponent()->RemoveItem(InventoryPosition);
		}
	}
	else
	{
		int64 NewPosition = PlayerCharacter->GetInventoryComponent()->GetFirstEmptySlotPosition();

		if (PlayerCharacter->GetInventoryComponent()->GetItem(NewPosition).IsEmpty())
		{
			Data.bIsEquipped = false;
			PlayerCharacter->GetInventoryComponent()->AddItem(Data, NewPosition);
			PlayerCharacter->GetEquipmentComponent()->RemoveItem(InventoryPosition);
		}
	}
}
