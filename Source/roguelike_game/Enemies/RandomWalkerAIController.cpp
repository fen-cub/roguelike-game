// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomWalkerAIController.h"
#include "Chaser.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Fill out your copyright notice in the Description page of Project Settings.


void ARandomWalkerAIController::BeginPlay()
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

void ARandomWalkerAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ARandomWalkerAIController::GenerateRandomSearchLocation()
{
	RandomLocation = NavArea->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(), 100.0f);
}

void ARandomWalkerAIController::SearchForPlayer()
{
	MoveToLocation(RandomLocation);
}

void ARandomWalkerAIController::AttackPlayer()
{
}

void ARandomWalkerAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (bSearchForPlayer)
	{
		GenerateRandomSearchLocation();
		SearchForPlayer();
	}
}

void ARandomWalkerAIController::OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
}

void ARandomWalkerAIController::OnDetectPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
