// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStorageComponent.h"

#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "roguelike_game/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UItemStorageComponent::UItemStorageComponent()
{
	SetStorageSize(9);
	FirstEmptySlotPosition = 0;
}


// Called when the game starts
void UItemStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UItemStorageComponent::AddItem(FItemData Item)
{
	UE_LOG(LogTemp, Warning, TEXT("Storage size: %d"), static_cast<int>(StorageSize));
	if (StorageSize > 0 && FirstEmptySlotPosition != StorageSize)
	{
		ItemStorage[FirstEmptySlotPosition] = Item;
		UE_LOG(LogTemp, Warning, TEXT("Insert item on slot: %d"), static_cast<int>(FirstEmptySlotPosition));
		PlayerHUD->InsertItem(FirstEmptySlotPosition, Item);

		while (FirstEmptySlotPosition < StorageSize && !ItemStorage[FirstEmptySlotPosition].IsEmpty())
		{
			FirstEmptySlotPosition++;
		}
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("There are no empty slots in Storage"));
	}
}

void UItemStorageComponent::RemoveItem(int64 Position)
{
	check (Position >= 0 && Position < StorageSize);

	if (!ItemStorage[Position].IsEmpty())
	{
		ItemStorage[Position] = EmptySlot;
		UE_LOG(LogTemp, Warning, TEXT("Remove item on slot: %d"), static_cast<int>(Position));
		PlayerHUD->InsertItem(Position, EmptySlot);

		FirstEmptySlotPosition = FMath::Min(Position, FirstEmptySlotPosition);
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an empty slot"));
	}
}

void UItemStorageComponent::UseItem(int64 Position)
{
	TSubclassOf<AItem> Item = ItemStorage[Position].Class;

	if (Item)
	{
		AItem* CDOItem = Item.GetDefaultObject();
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
		if (CDOItem && PlayerCharacter)
		{
			CDOItem->Use(PlayerCharacter);
		}
		RemoveItem(Position);
	} else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an empty slot"));
	}
}

// Set Player's HUD
void UItemStorageComponent::SetUpHUD(UPlayerHUD* HUD)
{
	PlayerHUD = HUD;

	PlayerHUD->SetGridPanelSizes(1, StorageSize);

	for (int64 Position = 0; Position < StorageSize; ++Position)
	{
		PlayerHUD->InsertItem(Position, ItemStorage[Position]);
	} 
}

void UItemStorageComponent::SetStorageSize(int64 Size)
{
	StorageSize = Size;

	ItemStorage.Init(EmptySlot, StorageSize);
}
