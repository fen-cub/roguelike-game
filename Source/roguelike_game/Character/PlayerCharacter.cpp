// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->InitCapsuleSize(10.0f, 10.0f);

	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 180.0f, 180.0f));
	CameraBoom->TargetArmLength = 80.0f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	bDead = false;
	bIsMoving = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::Animate);
}

void APlayerCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	auto MovementComponent = GetMovementComponent();

	if (MovementComponent)
	{
		MovementComponent->AddInputVector(WorldDirection * ScaleValue, bForce);
	}
	else
	{
		Internal_AddMovementInput(WorldDirection * ScaleValue, bForce);
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::SetCurrentAnimationDirection(FVector const& Velocity)
{
	const float x = Velocity.GetSafeNormal().X;
	const float y = Velocity.GetSafeNormal().Y;

	bIsMoving = x != 0.0f || y != 0.0f;

	if (bIsMoving)
	{
		if (y > 0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Right;
			GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
		else if (y < -0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Left;
			GetSprite()->SetRelativeScale3D(FVector(-1.0f, 1.0f, 1.0f));
		}
		else if (x < -0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Down;
			GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
		else if (x > 0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Up;
			GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
		}
	}
}

void APlayerCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	SetCurrentAnimationDirection(OldVelocity);

	if (OldVelocity.Size() > 0.0f)
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.WalkUp);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.WalkDown);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.WalkRight);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.WalkRight);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.IdleUp);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.IdleDown);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.IdleRight);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.IdleRight);
			break;
		default:
			break;
		}
	}
}

void APlayerCharacter::MoveForward(float Axis)
{
	if ((Controller != nullptr) && (Axis != 0) && !bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis, true);
	}
}

void APlayerCharacter::MoveRight(float Axis)
{
	if ((Controller != nullptr) && (Axis != 0) && !bDead)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis, true);
	}
}
