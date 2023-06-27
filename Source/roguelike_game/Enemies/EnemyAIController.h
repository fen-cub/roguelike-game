// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	// Player
	APawn* PlayerPawn;
	
	// nav mesh area
	class UNavigationSystemV1* NavArea;

	// location to move on
	FVector RandomLocation = FVector();
	
	bool bIsMoving;
	
	void GenerateRandomSearchLocation();

	// keep looking for player character
	void SearchForPlayer();

	// timers for cooldown
	
public:
	// stuff to continue after location reached
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
};
