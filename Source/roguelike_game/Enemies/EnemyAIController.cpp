// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	bSearchForPlayer = true;
	bCanAttackPlayer = true;
	bMoveToPlayer = false;

	GenerateRandomSearchLocation();
	SearchForPlayer();
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AEnemyAIController::GenerateRandomSearchLocation()
{
	RandomLocation = NavArea->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(), 50.0f);
}

void AEnemyAIController::SearchForPlayer()
{
	MoveToLocation(RandomLocation);
}

void AEnemyAIController::MoveToPlayer()
{
	if (PlayerPawn && bCanAttackPlayer)
	{
		bMoveToPlayer = true;
		bSearchForPlayer = false;

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PlayerPawn);
		MoveRequest.SetAcceptanceRadius(100.0f);

		MoveTo(MoveRequest);
	}
}

void AEnemyAIController::AttackPlayer()
{
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bSearchForPlayer)
	{
		GenerateRandomSearchLocation();
		SearchForPlayer();
	}

	if (FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation()) < 200.0f)
	{
		bSearchForPlayer = false;
		MoveToLocation(PlayerPawn->GetActorLocation());
	}
}

void AEnemyAIController::OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
}

void AEnemyAIController::OnDetectPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
