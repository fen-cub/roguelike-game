// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomWalkerAIController.h"
#include "Chaser.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Fill out your copyright notice in the Description page of Project Settings.


void ARandomWalkerAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	
	bIsMoving = false;
}

void ARandomWalkerAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!NavArea)
	{
		NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	}

	if (!bIsMoving && NavArea && GetPawn())
	{
		SearchForPlayer();
	}
}

void ARandomWalkerAIController::GenerateRandomSearchLocation()
{
	if (NavArea && GetPawn())
	{
		RandomLocation = NavArea->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(), 50.0f);
	} 
}

void ARandomWalkerAIController::SearchForPlayer()
{
	if (NavArea && GetPawn())
	{
		GenerateRandomSearchLocation();
		if ( FVector::Dist(RandomLocation, GetPawn()->GetActorLocation()) > 10.0f )
		{
			bIsMoving = true;
			MoveToLocation(RandomLocation); 
		} else
		{
			bIsMoving = false;
		}
	}
}
void ARandomWalkerAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	SearchForPlayer();
}
