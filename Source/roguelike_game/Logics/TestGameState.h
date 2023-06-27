// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TestGameState.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API ATestGameState : public AGameState
{
	GENERATED_BODY()

public:
	void GameOver(bool bIsWon) const;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Level)
	int LevelNumber;

	//UFUNCTION(Server, Reliable)
	bool OpenNextLevel();
	
};
