// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Components/PanelSlot.h"
#include "InventorySlot.h"
#include "Components/ProgressBar.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/UniformGridPanel.h"

int64 UPlayerHUD::GetRow(int64 Position) const
{
	return Position / Columns + 1;
}

int64 UPlayerHUD::GetColumn(int64 Position) const
{
	return Position % Columns + 1;
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

void UPlayerHUD::SetGridPanelSizes(int64 RowCount, int64 ColumnCount)
{
	Rows = RowCount;
	Columns = ColumnCount;
}
