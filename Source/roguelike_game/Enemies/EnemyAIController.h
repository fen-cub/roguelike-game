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

	// the mob itself
	class AEnemyCharacter* ControlledEnemy;

	// nav mesh area
	class UNavigationSystemV1* NavArea;

	// location to move on
	FVector RandomLocation = FVector();

	bool bMoveToPlayer;
	bool bSearchForPlayer;
	bool bCanAttackPlayer;
	bool bIsMoving;

	float PlayerStoppingDistance = 60.0f;
	float PlayerAttackDistance = 200.0f;

	void GenerateRandomSearchLocation();

	// keep looking for player character
	void SearchForPlayer();

	// moving exactly to player's location
	void MoveToPlayer();

	void StartChasing();
	void AttackPlayer();

	// timers for cooldown
	FTimerHandle SearchTimerHandle;
	FTimerHandle AttackTimerPlayer;

public:
	// stuff to continue after location reached
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


	// if overlap
	UFUNCTION()
	void OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                const FHitResult& SweepResult);

	// end overlap
	UFUNCTION()
	void OnDetectPlayerEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	                              class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
