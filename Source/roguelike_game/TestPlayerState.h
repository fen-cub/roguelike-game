// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GameOverWidget.h"
#include "TestPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API ATestPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void GameOver(bool bIsWon);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	
};
