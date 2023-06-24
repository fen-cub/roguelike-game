// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemStorageComponent.h"

#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/Widgets/Inventory.h"
#include "roguelike_game/Character/PlayerCharacter.h"

// Sets default values for this component's properties
UItemStorageComponent::UItemStorageComponent()
{
	SetIsReplicated(true);
	StorageSize = 9;
	FirstEmptySlotPosition = 0;

	ItemRandomStorage.Init(EmptySlot, 0);
	bIsGenerated = false;
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

	DOREPLIFETIME(UItemStorageComponent, bIsGenerated);
}

void UItemStorageComponent::ServerAddItem_Implementation(FItemData Item, int64 Position)
{
	OnRep_AddItem(Item, Position);
}

void UItemStorageComponent::OnRep_AddItem_Implementation(FItemData Item, int64 Position)
{
	check(Position >= 0 && Position <= StorageSize);
	
	if (StorageSize > 0 && Position != StorageSize && ItemStorage[Position].IsEmpty())
	{
		ItemStorage[Position] = Item;
		if (InventoryWidget)
		{
			InventoryWidget->SetItem(Position, Item);
		}

		while (FirstEmptySlotPosition < StorageSize && !ItemStorage[FirstEmptySlotPosition].IsEmpty())
		{
			FirstEmptySlotPosition++;
		}
	}
}

void UItemStorageComponent::ServerRemoveItem_Implementation(int64 Position)
{
	OnRep_RemoveItem(Position);
}

void UItemStorageComponent::OnRep_RemoveItem_Implementation(int64 Position)
{
	check(Position >= 0 && Position < StorageSize);

	if (!ItemStorage[Position].IsEmpty())
	{
		ItemStorage[Position] = EmptySlot;
		if (InventoryWidget)
		{
			InventoryWidget->SetItem(Position, EmptySlot);
		}

		FirstEmptySlotPosition = FMath::Min(Position, FirstEmptySlotPosition);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an empty slot"));
	}
}

void UItemStorageComponent::ServerUseItem_Implementation(int64 Position)
{
	OnRep_UseItem(Position);
}

void UItemStorageComponent::OnRep_UseItem_Implementation(int64 Position)
{
	if (!ItemStorage[Position].IsEmpty())
	{
		TSubclassOf<AItem> Item = ItemStorage[Position].Class;

		if (Item)
		{
			AItem* CDOItem = Item.GetDefaultObject();
			CDOItem->SetItemData(ItemStorage[Position]);

			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
			if (CDOItem && PlayerCharacter)
			{
				CDOItem->Use(PlayerCharacter, Position);
			}
		}
	}
}

int64 UItemStorageComponent::GetNextRandomInteger(uint64 LastRandom) const
{
	uint64 UpperBound = 1ULL << 30ULL;
	return (LastRandom * 1103515245ULL + 12345ULL) % UpperBound;
}

int64 UItemStorageComponent::GetRandomInRange(int64 Left, int64 Right, int64 RandomNumber) const
{
	return Left + (RandomNumber) % (Right - Left + 1);
}

void UItemStorageComponent::AddItem(FItemData Item, int64 Position)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerAddItem(Item, Position);
	}
}

void UItemStorageComponent::RemoveItem(int64 Position)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerRemoveItem(Position);
	}
}

void UItemStorageComponent::UseItem(int64 Position)
{
	if (GetOwner()->HasLocalNetOwner())
	{
		ServerUseItem(Position);
	}
}

FItemData UItemStorageComponent::GetItem(int64 Position)
{
	if (0 <= Position && Position < GetStorageSize())
	{
		return ItemStorage[Position];
	}

	return EmptySlot;
}

void UItemStorageComponent::GenerateRandomContents(int64 RandomHash)
{
	for (int64 Position = 0; Position < StorageSize; ++Position)
	{
		RandomHash = GetNextRandomInteger(RandomHash);
		int64 Chance = GetRandomInRange(1, 100, RandomHash);

		if (Chance <= 20 && ItemRandomStorage.Num() > 0)
		{
			RandomHash = GetNextRandomInteger(RandomHash);
			const int64 RandomItemPosition = GetRandomInRange(0, ItemRandomStorage.Num() - 1, RandomHash);
			ItemStorage[Position] = ItemRandomStorage[RandomItemPosition];
		}
	}

	while (FirstEmptySlotPosition < StorageSize && !ItemStorage[FirstEmptySlotPosition].IsEmpty())
	{
		FirstEmptySlotPosition++;
	}
	bIsGenerated = true;
}

void UItemStorageComponent::SetUpInventoryWidget(UInventory* Widget)
{
	InventoryWidget = Widget;
	
	for (int64 Position = 0; Position < StorageSize; ++Position)
	{
		InventoryWidget->InsertItem(Position, ItemStorage[Position]);
	}
}


void UItemStorageComponent::SetStorageSize(int64 Size)
{
	StorageSize = Size;

	ItemStorage.Init(EmptySlot, StorageSize);
}

int64 UItemStorageComponent::GetStorageSize() const
{
	return StorageSize;
}

int64 UItemStorageComponent::GetFirstEmptySlotPosition() const
{
	return FirstEmptySlotPosition;
}
