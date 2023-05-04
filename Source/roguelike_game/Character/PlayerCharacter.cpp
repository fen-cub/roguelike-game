// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Set default player settings
APlayerCharacter::APlayerCharacter()
{
	// Default logic settings
	bIsDead = false;

	// Default rotation settings
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Default movement settings
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	bIsMoving = false;


	// Default capsule component settings
	GetCapsuleComponent()->InitCapsuleSize(10.0f, 10.0f);

	// Default sprite settings
	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	// Default camera boom settings
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 180.0f, 180.0f));
	CameraBoom->TargetArmLength = 80.0f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// Default camera settings
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

// Called when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Animate character on movement
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::Animate);
}

// Called when moves
void APlayerCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	GetMovementComponent()->AddInputVector(WorldDirection * ScaleValue, bForce);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveUpOrDown", this, &APlayerCharacter::MoveForwardOrDown);
	PlayerInputComponent->BindAxis("MoveRightOrLeft", this, &APlayerCharacter::MoveRightOrLeft);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &APlayerCharacter::Die);
}

// Called every animation while alive
void APlayerCharacter::SetCurrentAnimationDirection(FVector const& Velocity)
{
	const float x = Velocity.GetSafeNormal().X;
	const float y = Velocity.GetSafeNormal().Y;

	bIsMoving = !FMath::IsNearlyZero(x, ComparisonErrorTolerance) || !FMath::IsNearlyZero(y, ComparisonErrorTolerance);

	if (bIsMoving)
	{
		if (y > 0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Right;
		}
		else if (y < -0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Left;
		}
		else if (x < -0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Down;
		}
		else if (x > 0.5f)
		{
			CurrentAnimationDirection = EAnimationDirection::Up;
		}
	}
}

// Called while alive or dying
void APlayerCharacter::Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	if (bIsDead)
	{
		GetSprite()->SetLooping(false);
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.DieRight);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.DieLeft);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.DieLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.DieRight);
			break;
		default:
			break;
		}

		// Removes animating on movement
		OnCharacterMovementUpdated.RemoveDynamic(this, &APlayerCharacter::Animate);
		return;
	}

	SetCurrentAnimationDirection(OldVelocity);

	// If standing still
	if (FMath::IsNearlyZero(OldVelocity.Size(), ComparisonErrorTolerance))
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
			GetSprite()->SetFlipbook(Flipbooks.IdleLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.IdleRight);
			break;
		default:
			break;
		}
	}
	// If sprinting
	else if (FMath::IsNearlyEqual(GetCharacterMovement()->MaxWalkSpeed, 200.0f, ComparisonErrorTolerance))
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.RunUp);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.RunLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.RunRight);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.RunDown);
			break;
		default:
			break;
		}
	}
	// If walking
	else
	{
		switch (CurrentAnimationDirection)
		{
		case EAnimationDirection::Up:
			GetSprite()->SetFlipbook(Flipbooks.WalkUp);
			break;
		case EAnimationDirection::Left:
			GetSprite()->SetFlipbook(Flipbooks.WalkLeft);
			break;
		case EAnimationDirection::Right:
			GetSprite()->SetFlipbook(Flipbooks.WalkRight);
			break;
		case EAnimationDirection::Down:
			GetSprite()->SetFlipbook(Flipbooks.WalkDown);
			break;
		default:
			break;
		}
	}
}

// Called when W or S keys pressed
void APlayerCharacter::MoveForwardOrDown(const float Axis)
{
	if ((Controller != nullptr) && !bIsDead && !FMath::IsNearlyZero(Axis, ComparisonErrorTolerance))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis, true);
	}
}

// Called when A or D keys pressed
void APlayerCharacter::MoveRightOrLeft(const float Axis)
{
	if ((Controller != nullptr) && !bIsDead && !FMath::IsNearlyZero(Axis, ComparisonErrorTolerance))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis, true);
	}
}

// Called when shift pressed
void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}

// Called when shift released
void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

// Called when dying 
void APlayerCharacter::Die()
{
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
