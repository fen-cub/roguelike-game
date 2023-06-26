// Fill out your copyright notice in the Description page of Project Settings.


#include "Chaser.h"

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

AChaser::AChaser()
{
	bIsDead = false;
	bIsMoving = false;
	PrimaryActorTick.bCanEverTick = true;
	ComparisonErrorTolerance = 1e-7;
	StaminaRegenerateRate = 0.25f;
	RunningStaminaLossRate = -0.5f;

	Health = 50;
	MaxHealth = 50;

	WalkSpeed = 100.0f;

	DamageDealt = 10.0f;

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

USphereComponent* AChaser::GetDetectPlayerCollisionSphere()
{
	return DetectPlayerCollisionSphere;
}


// Called when spawned
void AChaser::BeginPlay()
{
	Super::BeginPlay();

	// Animate character on movement
	OnCharacterMovementUpdated.AddDynamic(this, &AChaser::UpdateMovementProperties);
	
}

// Called when dying or in the end
void AChaser::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Replicate variables on the server
void AChaser::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AChaser, bIsMoving);
	DOREPLIFETIME(AChaser, bIsDead);
}

// Called when moves
void AChaser::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	GetMovementComponent()->AddInputVector(WorldDirection * ScaleValue, bForce);
}

// Called to bind functionality to input
void AChaser::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUpOrDown", this, &AChaser::MoveForwardOrDown);
	PlayerInputComponent->BindAxis("MoveRightOrLeft", this, &AChaser::MoveRightOrLeft);
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &AChaser::Die);
}

void AChaser::UpdateMovementProperties(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	AnimationComponent->SetCurrentCharacterDirection(OldVelocity);

	bIsMoving = !FMath::IsNearlyZero(OldVelocity.Size(), ComparisonErrorTolerance);

	if (bIsMoving && !bIsDead)
	{
		// AnimationComponent->AnimateWalking();
	}
	else if (!bIsDead)
	{
		// AnimationComponent->AnimateIdle();
	}
}

// Called when W or S keys are pressed
void AChaser::MoveForwardOrDown(const float Axis)
{
	if ((Controller != nullptr) && !bIsDead && !FMath::IsNearlyZero(Axis, ComparisonErrorTolerance))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis, true);
	}
}

// Called when A or D keys are pressed
void AChaser::MoveRightOrLeft(const float Axis)
{
	if ((Controller != nullptr) && !bIsDead && !FMath::IsNearlyZero(Axis, ComparisonErrorTolerance))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis, true);
	}
}


void AChaser::Die()
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


// Called when client dying
void AChaser::ServerSetDying_Implementation()
{
	Die();
}

// Calls back from server when dying
void AChaser::OnRep_IsDead()
{
	// AnimationComponent->AnimateDeath();
	EndPlay(EEndPlayReason::Destroyed);
}

// Called every frame
void AChaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerPawn->GetName());
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
		if (FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) <
			30.0f && AttackTickCount >= 100)
		{
			PlayerCharacter->GetAttributesComponent()->DamageCharacter(DamageDealt);

			AttackTickCount = 0;
		}
		if (PlayerCharacter && PlayerCharacter->bIsAttacking && FVector::Dist(
				GetActorLocation(), PlayerPawn->GetActorLocation()) <
			30.0f && DamageTickCount >= 100)
		{
			Health -= PlayerCharacter->GetDamageDealt();
			if (Health <= 0)
			{
				Destroy();
			}

			DamageTickCount = 0;
		}
	}


	UEnemyHealthBar* HealthBarWidget = Cast<UEnemyHealthBar>(WidgetComponent->GetWidget());
	if (HealthBarWidget && !FMath::IsNearlyZero(MaxHealth))
	{
		HealthBarWidget->SetHealth(Health, MaxHealth);
	}

	AttackTickCount++;
	DamageTickCount++;
}
