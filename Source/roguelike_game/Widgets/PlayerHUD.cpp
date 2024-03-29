// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "StorageDisplay.h"
#include "Components/ProgressBar.h"

UStorageDisplay* UPlayerHUD::GetInteractableStorageWidget() const
{
	return InteractableStorageWidget;
}

void UPlayerHUD::SetInteractableStorageWidget(UStorageDisplay* const NewInteractableStorageWidget)
{
	InteractableStorageWidget = NewInteractableStorageWidget;
}

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerHUD::SetStamina(float CurrentStamina, float MaxStamina)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentStamina / MaxStamina);
	}
}

UProgressBar* UPlayerHUD::GetHealthBar() const
{
	return HealthBar;
}

UProgressBar* UPlayerHUD::GetStaminaBar() const
{
	return StaminaBar;
}

UInventory* UPlayerHUD::GetInventoryWidget() const
{
	return InventoryWidget;
}

UEquipmentWidget* UPlayerHUD::GetEquipmentWidget() const
{
	return EquipmentWidget;
}
