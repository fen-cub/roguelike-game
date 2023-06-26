// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "roguelike_game/Interfaces/DisplayInterface.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UEnemyHealthBar : public UUserWidget, public IDisplayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;
	
	virtual void SetHealth(float CurrentHealth, float MaxHealth) override;
	
	virtual void SetStamina(float CurrentStamina, float MaxStamina) override;
};
