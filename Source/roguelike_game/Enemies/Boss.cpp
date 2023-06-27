// Fill out your copyright notice in the Description page of Project Settings.

// boss logic, controller is the EnemyAIController


#include "Boss.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "roguelike_game/Components/CharacterAnimationComponent.h"
#include "roguelike_game/Components/CharacterAttributesComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "../Widgets/EnemyHealthBar.h"
#include "Components/WidgetComponent.h"
#include "roguelike_game/Character/PlayerCharacter.h"

ABoss::ABoss()
{
	bIsDead = false;
	bIsMoving = false;

	Health = 100;
	MaxHealth = 100;
	DamageDealt = 20.0f;

	DetectPlayerCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	DetectPlayerCollisionSphere->SetupAttachment(RootComponent);

	GetCapsuleComponent()->InitCapsuleSize(10.0f, 10.0f);

	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	AnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>("Animation Component");
	AnimationComponent->SetupAttachment(RootComponent);
	AnimationComponent->SetupOwner(GetSprite());
	AnimationComponent->SetupAttachment(RootComponent);

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Component");
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 180.0f));
	WidgetComponent->SetRelativeScale3D(FVector(0.025f, 0.025f, 0.025f));
	WidgetComponent->SetRelativeLocation(FVector(10.0f, 0.0f, 0.0f));
}

USphereComponent* ABoss::GetDetectPlayerCollisionSphere()
{
	return DetectPlayerCollisionSphere;
}


void ABoss::BeginPlay()
{
	Super::BeginPlay();
}

void ABoss::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ABoss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABoss, bIsMoving);
	DOREPLIFETIME(ABoss, bIsDead);
}

void ABoss::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	GetMovementComponent()->AddInputVector(WorldDirection * ScaleValue, bForce);
}


void ABoss::Die()
{
	if (!HasAuthority())
	{
		ServerSetDying();
	}
	else
	{
		bIsDead = true;
		OnRep_IsDead();
	}
}


void ABoss::ServerSetDying_Implementation()
{
	Die();
}

void ABoss::OnRep_IsDead()
{
	EndPlay(EEndPlayReason::Destroyed);
}

// Called every frame
void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> FoundPlayers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), FoundPlayers);
	
	APawn* PlayerPawn = nullptr;

	for (auto Player : FoundPlayers)
	{
		if (FVector::Dist(GetActorLocation(), Player->GetActorLocation()) < 100.0f &&
			(PlayerPawn == nullptr || FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <
				FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation())))
		{
			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player);
			if (PlayerCharacter)
			{
				PlayerPawn = PlayerCharacter;
			}
		}
	}

	if (PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerPawn->GetName());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);

		// logic for attack of boss
		if (FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) <
			30.0f && AttackTickCount >= 100)
		{
			PlayerCharacter->GetAttributesComponent()->DamageCharacter(3 * DamageDealt);

			AttackTickCount = 0;
		}

		// logic for attack of character
		if (PlayerCharacter && PlayerCharacter->bIsAttacking && FVector::Dist(
				GetActorLocation(), PlayerPawn->GetActorLocation()) <
			30.0f && DamageTickCount >= 100)
		{
			Health -= PlayerCharacter->GetDamageDealt() / 3;
			if (Health <= 0)
			{
				Destroy();
			}

			DamageTickCount = 0;
		}
	}

	// health bar widget
	UEnemyHealthBar* HealthBarWidget = Cast<UEnemyHealthBar>(WidgetComponent->GetWidget());
	if (HealthBarWidget && !FMath::IsNearlyZero(MaxHealth))
	{
		HealthBarWidget->SetHealth(Health, MaxHealth);
	}

	AttackTickCount++;
	DamageTickCount++;
}
