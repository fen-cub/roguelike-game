// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Set default player settings
APlayerCharacter::APlayerCharacter()
{
	// Default logic settings
	bIsDead = false;
	bIsMoving = false;
	CurrentCharacterDirection = ECharacterDirection::Down;
	
	// HUD
	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;

	// Health
	MaxHealth = 25.f;
	Health = MaxHealth;

	// Stamina
	MaxStamina = 25.f;
	Stamina = MaxStamina;
	PowerRegenerateRate = 1.f;
	
	// Default rotation settings
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Default movement settings
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

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
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::AnimateMovement);
	
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		APlayerController* Fpc = GetController<APlayerController>();
		check(Fpc);
		PlayerHUD = CreateWidget<UPlayerHUD>(Fpc, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->AddToPlayerScreen();
		PlayerHUD->SetHealth(Health, MaxHealth);
		PlayerHUD->SetStamina(Stamina, MaxStamina);
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APlayerCharacter, Health, COND_OwnerOnly);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		// We can't destroy the widget directly, let the GC take care of it.
		PlayerHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
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
void APlayerCharacter::SetCurrentCharacterDirection(FVector const& Velocity)
{
	const float x = Velocity.GetSafeNormal().X;
	const float y = Velocity.GetSafeNormal().Y;

	bIsMoving = !FMath::IsNearlyZero(x, ComparisonErrorTolerance) || !FMath::IsNearlyZero(y, ComparisonErrorTolerance);

	if (bIsMoving)
	{
		if (y > 0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Right;
		}
		else if (y < -0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Left;
		}
		else if (x < -0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Down;
		}
		else if (x > 0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Up;
		}
	}
}

// Called while moves
void APlayerCharacter::AnimateMovement(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	if (FMath::IsNearlyZero(Stamina, ComparisonErrorTolerance))
	{
		GetCharacterMovement()->MaxWalkSpeed = 100.0f;
	}

	SetCurrentCharacterDirection(OldVelocity);

	// If standing still
	if (FMath::IsNearlyZero(OldVelocity.Size(), ComparisonErrorTolerance))
	{
		switch (CurrentCharacterDirection)
		{
		case ECharacterDirection::Up:
			GetSprite()->SetFlipbook(IdleFlipbooks.IdleUp);
			break;
		case ECharacterDirection::Down:
			GetSprite()->SetFlipbook(IdleFlipbooks.IdleDown);
			break;
		case ECharacterDirection::Left:
			GetSprite()->SetFlipbook(IdleFlipbooks.IdleLeft);
			break;
		case ECharacterDirection::Right:
			GetSprite()->SetFlipbook(IdleFlipbooks.IdleRight);
			break;
		default:
			break;
		}
	}
	// If sprinting
	else if (FMath::IsNearlyEqual(GetCharacterMovement()->MaxWalkSpeed, 200.0f, ComparisonErrorTolerance))
	{
		Stamina -= 0.05f;;
		if (PlayerHUD)
		{
			PlayerHUD->SetStamina(Stamina, MaxStamina);
		}
		switch (CurrentCharacterDirection)
		{
		case ECharacterDirection::Up:
			GetSprite()->SetFlipbook(RunningFlipbooks.RunUp);
			break;
		case ECharacterDirection::Left:
			GetSprite()->SetFlipbook(RunningFlipbooks.RunLeft);
			break;
		case ECharacterDirection::Right:
			GetSprite()->SetFlipbook(RunningFlipbooks.RunRight);
			break;
		case ECharacterDirection::Down:
			GetSprite()->SetFlipbook(RunningFlipbooks.RunDown);
			break;
		default:
			break;
		}
	}
	// If walking
	else
	{
		switch (CurrentCharacterDirection)
		{
		case ECharacterDirection::Up:
			GetSprite()->SetFlipbook(WalkingFlipbooks.WalkUp);
			break;
		case ECharacterDirection::Left:
			GetSprite()->SetFlipbook(WalkingFlipbooks.WalkLeft);
			break;
		case ECharacterDirection::Right:
			GetSprite()->SetFlipbook(WalkingFlipbooks.WalkRight);
			break;
		case ECharacterDirection::Down:
			GetSprite()->SetFlipbook(WalkingFlipbooks.WalkDown);
			break;
		default:
			break;
		}
	}
}

// Called when dying
void APlayerCharacter::AnimateDeath()
{
	GetSprite()->SetLooping(false);
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		GetSprite()->SetFlipbook(DeathFlipbooks.DieRight);
		break;
	case ECharacterDirection::Down:
		GetSprite()->SetFlipbook(DeathFlipbooks.DieLeft);
		break;
	case ECharacterDirection::Left:
		GetSprite()->SetFlipbook(DeathFlipbooks.DieLeft);
		break;
	case ECharacterDirection::Right:
		GetSprite()->SetFlipbook(DeathFlipbooks.DieRight);
		break;
	default:
		break;
	}

	// Removes Movement Animating 
	OnCharacterMovementUpdated.RemoveDynamic(this, &APlayerCharacter::AnimateMovement);
}

// Called when W or S keys are pressed
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

// Called when A or D keys are pressed
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

// Called when shift is pressed
void APlayerCharacter::Sprint()
{
	if (!FMath::IsNearlyZero(Stamina, ComparisonErrorTolerance))
	{
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	}
}

// Called when shift is released
void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;
}

// Called when dying 
void APlayerCharacter::Die()
{
	bIsDead = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	AnimateDeath();
}


void APlayerCharacter::OnRepHealth()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetHealth(Health, MaxHealth);
	}
}

void APlayerCharacter::RegenerateStamina()
{
	Stamina = FMath::Clamp( Stamina + (PowerRegenerateRate * GetWorld()->GetDeltaSeconds()), 0.f, MaxStamina);
	if (PlayerHUD)
	{
		PlayerHUD->SetStamina(Stamina, MaxStamina);
	}
}

void APlayerCharacter::UpdateHealth(float HealthDelta)
{
	Health = FMath::Clamp(Health + HealthDelta, 0.f, MaxHealth);

	if (Health == 0.f)
	{
		// Handle player elimination.
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RegenerateStamina();
}
