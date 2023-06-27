// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomWalker.h"
#include "roguelike_game/Components/CharacterAttributesComponent.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/CharacterAnimationComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"

ARandomWalker::ARandomWalker()
{
	bIsDead = false;
	bIsMoving = false;
	PrimaryActorTick.bCanEverTick = true;
	ComparisonErrorTolerance = 1e-7;
	StaminaRegenerateRate = 0.25f;
	RunningStaminaLossRate = -0.5f;

	Health = 50;

	WalkSpeed = 100.0f;

	DetectPlayerCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	DetectPlayerCollisionSphere->SetupAttachment(RootComponent);

	GetCapsuleComponent()->InitCapsuleSize(10.0f, 10.0f);

	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	AnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>("Animation Component");
	AnimationComponent->SetupAttachment(RootComponent);
	AnimationComponent->SetupOwner(GetSprite());
	AnimationComponent->SetupAttachment(RootComponent);
}

USphereComponent* ARandomWalker::GetDetectPlayerCollisionSphere()
{
	return DetectPlayerCollisionSphere;
}


// Called when spawned
void ARandomWalker::BeginPlay()
{
	Super::BeginPlay();

	// Animate character on movement
	OnCharacterMovementUpdated.AddDynamic(this, &ARandomWalker::UpdateMovementProperties);
}

// Called when dying or in the end
void ARandomWalker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Replicate variables on the server
void ARandomWalker::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARandomWalker, bIsMoving);
	DOREPLIFETIME(ARandomWalker, bIsDead);
}

// Called when moves
void ARandomWalker::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	GetMovementComponent()->AddInputVector(WorldDirection * ScaleValue, bForce);
}

// Called to bind functionality to input
void ARandomWalker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUpOrDown", this, &ARandomWalker::MoveForwardOrDown);
	PlayerInputComponent->BindAxis("MoveRightOrLeft", this, &ARandomWalker::MoveRightOrLeft);
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &ARandomWalker::Die);
}

void ARandomWalker::UpdateMovementProperties(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	// AnimationComponent->SetCurrentCharacterDirection(OldVelocity);

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
void ARandomWalker::MoveForwardOrDown(const float Axis)
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
void ARandomWalker::MoveRightOrLeft(const float Axis)
{
	if ((Controller != nullptr) && !bIsDead && !FMath::IsNearlyZero(Axis, ComparisonErrorTolerance))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis, true);
	}
}


void ARandomWalker::Die()
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
void ARandomWalker::ServerSetDying_Implementation()
{
	Die();
}

// Calls back from server when dying
void ARandomWalker::OnRep_IsDead()
{
	// AnimationComponent->AnimateDeath();
	EndPlay(EEndPlayReason::Destroyed);
}

// Called every frame
void ARandomWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UE_LOG(LogTemp, Warning, TEXT("BatHealth %f"), Health);

	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(PlayerPawn);
	if (FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) <
		25.0f)	
	{
		constexpr float DamageAmount = 1.0f;
		PlayerCharacter->GetAttributesComponent()->UpdateHealth(-DamageAmount);
		if (PlayerCharacter->bIsAttacking && FVector::Dist(GetActorLocation(), PlayerPawn->GetActorLocation()) <
			50.0f)
		{
			Health -= 50.0;

			UE_LOG(LogTemp, Warning, TEXT("BatHealth %f"), Health);
			if (Health <= 0)
			{
				Destroy();
			}
		}
	}
}
