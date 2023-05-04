// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()


public:
	void SetHealth(float CurrentHealth, float MaxHealth);
	
	void SetStamina(float CurrentStamina, float MaxStamina);
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar;
};
