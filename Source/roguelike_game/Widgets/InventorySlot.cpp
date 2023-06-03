// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"
#include "roguelike_game/InteractiveActors/Storage.h"


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
	UInventory* InventoryWidget = Cast<UInventory>(GetParent()->GetOuter()->GetOuter());

	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Outer"));
	}

	if (InventoryWidget && !ItemData.IsEmpty())
	{
		switch (InventoryWidget->GetCurrentInventoryType())
		{
		case EInventoryType::PlayerHUDInventory:
			InteractButtonText->SetText(FText::FromString("Use"));
			break;
		case EInventoryType::PlayerInventoryInStorage:
			InteractButtonText->SetText(FText::FromString("Put"));
			break;
		case EInventoryType::StorageInventory:
			InteractButtonText->SetText(FText::FromString("Take"));
			break;
		default:
			break;
		}
		SetInteractButtonVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Set new clicked slot"));
	}

	InventoryWidget->SetNewClickedSlot(PositionInInventory);
}

void UInventorySlot::InteractButtonOnClicked()
{
	const UInventory* InventoryWidget = Cast<UInventory>(GetParent()->GetOuter()->GetOuter());

	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Outer"));
	}

	if (InventoryWidget)
	{
		if (InventoryWidget->GetCurrentInventoryType() == EInventoryType::PlayerHUDInventory)
		{
			InventoryWidget->GetOwnerStorage()->UseItem(PositionInInventory);
		}
		else
		{
			InventoryWidget->GetPairingStorage()->AddItem(
				ItemData, InventoryWidget->GetPairingStorage()->GetFirstEmptySlotPosition());
			InventoryWidget->GetOwnerStorage()->RemoveItem(PositionInInventory);
		}
	}

	InteractButton->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetInteractButtonVisibility(ESlateVisibility NewVisibility)
{
	InteractButton->SetVisibility(NewVisibility);
}
