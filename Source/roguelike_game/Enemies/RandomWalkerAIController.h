// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RandomWalkerAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API ARandomWalkerAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;

private:
	APawn* PlayerPawn;

	class AEnemyCharacter* ControlledEnemy;

	class UNavigationSystemV1* NavArea;

	FVector RandomLocation = FVector();

	bool bMoveToPlayer;
	bool bSearchForPlayer;
	bool bCanAttackPlayer;

	float PlayerStoppingDistance = 60.0f;
	float PlayerAttackDistance = 200.0f;

	void GenerateRandomSearchLocation();
	void SearchForPlayer();

	void AttackPlayer();

	FTimerHandle SearchTimerHandle;
	FTimerHandle AttackTimerPlayer;

public:
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION()
	void OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult);

	UFUNCTION()
	void OnDetectPlayerEndOverlap(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
								  class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
