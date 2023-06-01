// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"


void UInventorySlot::NativeConstruct()
{
	PositionInInventory = -1;
	
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UInventorySlot::ItemButtonOnClicked);
	}

	if (InteractButton)
	{
		InteractButton->OnClicked.AddDynamic(this, &UInventorySlot::InteractButtonOnClicked);
		InteractButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::ItemButtonOnClicked()
{
	if (InteractButton->GetVisibility() == ESlateVisibility::Hidden)
	{
		InteractButton->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		InteractButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UInventorySlot::InteractButtonOnClicked()
{
	if (ItemData.Owner)
	{
		APlayerCharacter* OwnerPlayer = Cast<APlayerCharacter>(ItemData.Owner);

		if (OwnerPlayer)
		{
			OwnerPlayer->GetInventoryComponent()->UseItem(PositionInInventory);
			InteractButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


