// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStorageComponent.h"

#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "roguelike_game/Widgets/Inventory.h"
#include "roguelike_game/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UItemStorageComponent::UItemStorageComponent()
{
	SetIsReplicated(true);
	FirstEmptySlotPosition = 0;
}


// Called when the game starts
void UItemStorageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UItemStorageComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

int64 UItemStorageComponent::GetFirstEmptySlotPosition() const
{
	return FirstEmptySlotPosition;
}

void UItemStorageComponent::AddItem(FItemData Item, int64 Position)
{
	check (Position >= 0 && Position <= StorageSize);
	
	UE_LOG(LogTemp, Warning, TEXT("Calls add item on client: %p"), this);
	if (StorageSize > 0 && Position != StorageSize && ItemStorage[Position].IsEmpty())
	{
		ItemStorage[Position] = Item;
		UE_LOG(LogTemp, Warning, TEXT("Insert item on slot: %d"), static_cast<int>(Position));
		if (InventoryWidget)
		{
			InventoryWidget->SetItem(Position, Item);
		}

		while (FirstEmptySlotPosition < StorageSize && !ItemStorage[FirstEmptySlotPosition].IsEmpty())
		{
			FirstEmptySlotPosition++;
		}
	} else if (InventoryWidget)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Slot is not empty or there are no available slots"));
	}
}

void UItemStorageComponent::RemoveItem(int64 Position)
{
	check (Position >= 0 && Position < StorageSize);
	
	if (!ItemStorage[Position].IsEmpty())
	{
		ItemStorage[Position] = EmptySlot;
		UE_LOG(LogTemp, Warning, TEXT("Remove item on slot: %d"), static_cast<int>(Position));
		if (InventoryWidget)
		{
			InventoryWidget->SetItem(Position, EmptySlot);
		}

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
	} 
}

void UItemStorageComponent::SetUpInventoryWidget(UInventory* Widget)
{
	InventoryWidget = Widget;

	UE_LOG(LogTemp, Warning, TEXT("Chest slot count: %d"), static_cast<int>(StorageSize));
	for (int64 Position = 0; Position < StorageSize; ++Position)
	{
		InventoryWidget->InsertItem(Position, ItemStorage[Position]);
	} 
}


void UItemStorageComponent::SetStorageSize(int64 Size)
{
	StorageSize = Size;

	UE_LOG(LogTemp, Warning, TEXT("Chest slot count: %d"), static_cast<int>(StorageSize));
	ItemStorage.Init(EmptySlot, StorageSize);
}

int64 UItemStorageComponent::GetStorageSize() const
{
	return StorageSize;
}

