// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySlot.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/ItemStorageComponent.h"
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
		case EInventoryType::EquipmentInventory:
			InteractButtonText->SetText(FText::FromString("Remove"));
			break;
		default:
			break;
		}
		SetInteractButtonVisibility(ESlateVisibility::Visible);
	}

	if (InventoryWidget)
	{
		InventoryWidget->SetNewClickedSlot(PositionInInventory);
	}
}

void UInventorySlot::InteractButtonOnClicked()
{
	const UInventory* InventoryWidget = Cast<UInventory>(GetParent()->GetOuter()->GetOuter());

	if (InventoryWidget)
	{
		if (InventoryWidget->GetCurrentInventoryType() == EInventoryType::PlayerHUDInventory
			|| InventoryWidget->GetCurrentInventoryType() == EInventoryType::EquipmentInventory)
		{
			InventoryWidget->GetOwnerStorage()->UseItem(PositionInInventory);
		}
		else
		{
			int64 NewPosition = InventoryWidget->GetPairingStorage()->GetFirstEmptySlotPosition();

			if (NewPosition < InventoryWidget->GetPairingStorage()->GetStorageSize())
			{
				InventoryWidget->GetPairingStorage()->AddItem(
					ItemData, NewPosition);
				InventoryWidget->GetOwnerStorage()->RemoveItem(PositionInInventory);
			}

			AActor* PlayerCharacter;
			if(InventoryWidget->GetCurrentInventoryType() == EInventoryType::PlayerInventoryInStorage) {
				PlayerCharacter = InventoryWidget->GetOwnerStorage()->GetOwner();
				UGameplayStatics::SpawnSoundAtLocation(PlayerCharacter, PutItemSound, PlayerCharacter->GetActorLocation());
			} else
			{
				PlayerCharacter = InventoryWidget->GetPairingStorage()->GetOwner();
				UGameplayStatics::SpawnSoundAtLocation(PlayerCharacter, TakeItemSound, PlayerCharacter->GetActorLocation());
			}
		}
	}

	InteractButton->SetVisibility(ESlateVisibility::Hidden);
}

void UInventorySlot::SetInteractButtonVisibility(ESlateVisibility NewVisibility)
{
	InteractButton->SetVisibility(NewVisibility);
}
