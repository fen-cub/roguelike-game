// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(Client, Reliable)
	void SetWidgetText(bool HasWon);

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton *ButtonToMenu;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *GameOverText;
	
};
