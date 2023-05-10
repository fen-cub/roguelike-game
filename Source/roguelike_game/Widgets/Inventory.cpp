// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"

int64 UInventory::GetRow(int64 Position) const
{
	return Position / Columns + 1;
}

int64 UInventory::GetColumn(int64 Position) const
{
	return Position % Columns + 1;
}

void UInventory::SetGridPanelSizes(int64 RowCount, int64 ColumnCount)
{
	Rows = RowCount;
	Columns = ColumnCount;
}
