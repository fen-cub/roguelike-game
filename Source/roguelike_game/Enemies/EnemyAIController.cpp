// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "NavigationSystem.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/CharacterAttributesComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	
	bIsMoving = false;
	
	UE_LOG(LogTemp, Warning, TEXT("Begin play in Enemy AI"));
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!NavArea)
	{
		NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	}

	if (!bIsMoving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Search for player in tick"));
		GenerateRandomSearchLocation();
		SearchForPlayer();
	}
}

void AEnemyAIController::GenerateRandomSearchLocation()
{
	if (NavArea && GetPawn())
	{
		RandomLocation = NavArea->GetRandomReachablePointInRadius(this, GetPawn()->GetActorLocation(), 50.0f);
	} 
}

void AEnemyAIController::SearchForPlayer()
{
	if (NavArea && GetPawn())
	{
		bIsMoving = true;
		FNavLocation NavLocation;

		TArray<AActor*> FoundPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayers);

		PlayerPawn = nullptr;
	
		for ( auto Player : FoundPlayers )
		{
			if ( FVector::Dist(GetPawn()->GetActorLocation(), Player->GetActorLocation()) < 100.0f &&
			(PlayerPawn == nullptr || FVector::Dist(GetPawn()->GetActorLocation(), Player->GetActorLocation()) <
				FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation()) ))
			{
				APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player);
				if (PlayerCharacter)
				{
					PlayerPawn = PlayerCharacter;
				}
			}
		}
	
		if (PlayerPawn ) {
			MoveToLocation(PlayerPawn->GetActorLocation());
		} else 
		{
			MoveToLocation(RandomLocation); 
		};
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	GenerateRandomSearchLocation();
	SearchForPlayer();
}

