// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAnimationComponent.h"
#include "Components/CharacterAttributesComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

// Set default player properties
APlayerCharacter::APlayerCharacter()
{
	// Default game logic properties
	bIsDead = false;
	bIsMoving = false;
	bIsSprinting = false;
	PrimaryActorTick.bCanEverTick = true;
	ComparisonErrorTolerance = 1e-7;
	StaminaRegenerateRate = 0.25f;
	RunningStaminaLossRate = -0.5f;

	// HUD
	PlayerHUDClass = nullptr;
	PlayerHUD = nullptr;

	// Default rotation properties
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Default movement properties
	SprintSpeed = 200.0f;
	WalkSpeed = 100.0f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Default capsule component properties
	GetCapsuleComponent()->InitCapsuleSize(10.0f, 10.0f);

	// Default sprite properties
	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	// Default camera boom properties
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetRelativeRotation(FRotator(-90.0f, 180.0f, 180.0f));
	CameraBoom->TargetArmLength = 80.0f;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// Default camera properties
	FollowCamera = CreateDefaultSubobject<UCameraComponent>("Follow Camera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Default animation properties
	AnimationComponent = CreateDefaultSubobject<UCharacterAnimationComponent>("Animation Component");
	AnimationComponent->SetupAttachment(RootComponent);
	AnimationComponent->SetupOwner(GetSprite());

	// Default attributes properties
	AttributesComponent = CreateDefaultSubobject<UCharacterAttributesComponent>("Attributes Component");
	AnimationComponent->SetupAttachment(RootComponent);
}

// Called when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Animate character on movement
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::UpdateMovementProperties);

	// Create and draw HUD
	if (IsLocallyControlled() && PlayerHUDClass)
	{
		APlayerController* Fpc = GetController<APlayerController>();
		check(Fpc);
		PlayerHUD = CreateWidget<UPlayerHUD>(Fpc, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->AddToPlayerScreen();
		AttributesComponent->SetUpHUD(PlayerHUD);
	}
}

// Called when dying or in the end
void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}

	Super::EndPlay(EndPlayReason);
}

// Replicate variables on the server
void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, bIsSprinting);
	DOREPLIFETIME(APlayerCharacter, bIsMoving);
	DOREPLIFETIME(APlayerCharacter, bIsDead);
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

void APlayerCharacter::UpdateMovementProperties(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	AnimationComponent->SetCurrentCharacterDirection(OldVelocity);

	bIsMoving = !FMath::IsNearlyZero(OldVelocity.Size(), ComparisonErrorTolerance);

	if (bIsMoving && !bIsDead)
	{
		if (bIsSprinting)
		{
			AnimationComponent->AnimateRunning();
			AttributesComponent->UpdateStamina(RunningStaminaLossRate);
		}
		else
		{
			AnimationComponent->AnimateWalking();
		}
	}
	else if (!bIsDead)
	{
		AnimationComponent->AnimateIdle();
	}
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
	if (!FMath::IsNearlyZero(AttributesComponent->GetStamina(), ComparisonErrorTolerance) && !bIsDead)
	{
		SetSprinting(true);
	}
}

// Called when shift is released
void APlayerCharacter::StopSprint()
{
	if (!bIsDead)
	{
		SetSprinting(false);
	}
}

// Called when dying 
void APlayerCharacter::Die()
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

// Called when start sprinting
void APlayerCharacter::SetSprinting(bool bNewSprinting)
{
	if (!HasAuthority())
	{
		ServerSetSprinting(bNewSprinting);
	}
	else
	{
		bIsSprinting = bNewSprinting;
		OnRep_IsSprinting();
	}
}

// Called when client start sprinting
void APlayerCharacter::ServerSetSprinting_Implementation(bool bNewSprinting)
{
	SetSprinting(bNewSprinting);
}

// Calls back from server when start sprinting
void APlayerCharacter::OnRep_IsSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

// Called when client dying
void APlayerCharacter::ServerSetDying_Implementation()
{
	Die();
}

// Calls back from server when dying
void APlayerCharacter::OnRep_IsDead()
{
	AnimationComponent->AnimateDeath();
	EndPlay(EEndPlayReason::Destroyed);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FMath::IsNearlyZero(AttributesComponent->GetHealth(), ComparisonErrorTolerance))
	{
		Die();
	}

	if (FMath::IsNearlyZero(AttributesComponent->GetStamina(), ComparisonErrorTolerance))
	{
		StopSprint();
	}

	AttributesComponent->UpdateStamina(StaminaRegenerateRate);
}
