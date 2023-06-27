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

	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	NavArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	bSearchForPlayer = true;
	bCanAttackPlayer = true;
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
	FNavLocation NavLocation;

	// beware to check player is near and is valid
	if (PlayerPawn && FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation()) < 100.0f && NavArea->
		ProjectPointToNavigation(PlayerPawn->GetActorLocation(), NavLocation, FVector::ZeroVector)
	)
	{
		bSearchForPlayer = false;
		MoveToLocation(PlayerPawn->GetActorLocation());
	}
	else // else go to random location
	{
		MoveToLocation(RandomLocation);
	};
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
	if (PlayerPawn && bCanAttackPlayer)
	{
		bMoveToPlayer = true;
		bSearchForPlayer = false;

		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(PlayerPawn);
		MoveRequest.SetAcceptanceRadius(100.0f);

		MoveTo(MoveRequest);

		// Deal damage to the player character
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
		if (PlayerCharacter != nullptr)
		{
			float DamageAmount = 20.0f;
			PlayerCharacter->GetAttributesComponent()->UpdateHealth(-10.0f);
		}
	}
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	if (bSearchForPlayer)
	{
		GenerateRandomSearchLocation();
		SearchForPlayer();
	}
	else if (PlayerPawn)
	{
		FNavLocation NavLocation;
		if (NavArea->ProjectPointToNavigation(PlayerPawn->GetActorLocation(), NavLocation, FVector::ZeroVector))
		{
			MoveToLocation(PlayerPawn->GetActorLocation());
			
		}
		else
		{
			bSearchForPlayer = true;
			GenerateRandomSearchLocation();
			SearchForPlayer();
		}
	}
}

void AEnemyAIController::OnDetectPlayerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                    bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateHeatlh %f"), PlayerCharacter->GetAttributesComponent()->GetHealth());

		// Handle the overlap with the player character
		// For example, start attacking the player or trigger other behavior
		AttackPlayer();
	}
}

void AEnemyAIController::OnDetectPlayerEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor);
	if (PlayerCharacter != nullptr)
	{
		// Handle the end of overlap with the player character
		// For example, stop attacking the player or perform other actions
	}
}
