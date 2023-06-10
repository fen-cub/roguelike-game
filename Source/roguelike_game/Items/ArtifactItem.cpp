// Fill out your copyright notice in the Description page of Project Settings.


#include "ArtifactItem.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"

void AArtifactItem::Use(APlayerCharacter* PlayerCharacter, int64 InventoryPosition)
{
	if (!Data.bIsEquipped)
	{
		if (PlayerCharacter->GetEquipmentComponent()->GetItem(2).IsEmpty())
		{
			Data.bIsEquipped = true;
			PlayerCharacter->GetEquipmentComponent()->AddItem(Data, 2);
			PlayerCharacter->GetInventoryComponent()->RemoveItem(InventoryPosition);
			ApplyArtifactProperties(PlayerCharacter);
		}
	} else
	{
		int64 NewPosition = PlayerCharacter->GetInventoryComponent()->GetFirstEmptySlotPosition();

		if (PlayerCharacter->GetInventoryComponent()->GetItem(NewPosition).IsEmpty())
		{
			Data.bIsEquipped = false;
			PlayerCharacter->GetInventoryComponent()->AddItem(Data, NewPosition);
			PlayerCharacter->GetEquipmentComponent()->RemoveItem(InventoryPosition);
			RemoveArtifactProperties(PlayerCharacter);
		}
	}
}
