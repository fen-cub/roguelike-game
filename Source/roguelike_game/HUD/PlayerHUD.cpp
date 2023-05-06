// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

#include "Components/PanelSlot.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UPlayerHUD::SetStamina(float CurrentPower, float MaxPower)
{
	if (StaminaBar)
	{
		StaminaBar->SetPercent(CurrentPower / MaxPower);
	}
}
