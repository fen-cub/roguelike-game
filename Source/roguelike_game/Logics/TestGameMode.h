// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "GameFramework/GameMode.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API ATestGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATestGameMode();

	virtual void PostLogin(APlayerController *NewPlayer) override;

	virtual void BeginPlay() override;

	protected:
	UPROPERTY()
	bool bIsMultiplayer = true;

	auto GetMyGameState() const;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Level)
	int LevelNumber;

};
