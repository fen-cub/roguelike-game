// Fill out your copyright notice in the Description page of Project Settings.


#include "StorageDisplay.h"

#include "Inventory.h"
#include "Components/UniformGridPanel.h"

UStorageDisplay::UStorageDisplay(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

UInventory* UStorageDisplay::GetInventoryWidget() const
{
	return InventoryWidget;
}
