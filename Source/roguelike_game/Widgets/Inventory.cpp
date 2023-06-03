// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

#include "InventorySlot.h"
#include "Components/UniformGridPanel.h"

void UInventory::SetOwnerStorage(UItemStorageComponent* const NewOwnerStorage)
{
	OwnerStorage = NewOwnerStorage;
}

void UInventory::SetPairingStorage(UItemStorageComponent* const NewPairingStorage)
{
	PairingStorage = NewPairingStorage;
}

void UInventory::SetNewClickedSlot(int64 Position)
{
	if (LastClickedSlotPosition != -1)
	{
		UInventorySlot* LastClickedSlot = Cast<UInventorySlot>(InventoryGridPanel->GetChildAt(LastClickedSlotPosition));

		if (LastClickedSlot)
		{
			LastClickedSlot->SetInteractButtonVisibility(ESlateVisibility::Hidden);
		}
	}
	LastClickedSlotPosition = Position;
}

int64 UInventory::GetRow(int64 Position) const
{
	return Position / Columns + 1;
}

int64 UInventory::GetColumn(int64 Position) const
{
	return Position % Columns + 1;
}

UItemStorageComponent* UInventory::GetOwnerStorage() const
{
	return OwnerStorage;
}

UItemStorageComponent* UInventory::GetPairingStorage() const
{
	return PairingStorage;
}

EInventoryType UInventory::GetCurrentInventoryType() const
{
	return CurrentInventoryType;
}

void UInventory::SetCurrentInventoryType(const EInventoryType NewInventoryType)
{
	this->CurrentInventoryType = NewInventoryType;
}

UUniformGridPanel* UInventory::GetInventoryGridPanel() const
{
	return InventoryGridPanel;
}

void UInventory::SetGridPanelSizes(int64 RowCount, int64 ColumnCount)
{
	Rows = RowCount;
	Columns = ColumnCount;
}
