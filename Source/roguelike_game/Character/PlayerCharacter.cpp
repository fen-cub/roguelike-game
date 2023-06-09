// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CharacterAnimationComponent.h"
#include "Components/CharacterAttributesComponent.h"
#include "roguelike_game/Items/Item.h"
#include "Components/InputComponent.h"
#include "Components/ItemStorageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/InteractiveActors/Storage.h"

// Set default player properties
APlayerCharacter::APlayerCharacter()
{
	// Default game logic properties
	bIsDead = false;
	bIsMoving = false;
	PrimaryActorTick.bCanEverTick = true;
	ComparisonErrorTolerance = 1e-4f;
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

	// Default trigger capsule properties
	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>("Trigger Capsule");
	TriggerCapsule->InitCapsuleSize(10.0f, 10.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

	// Default inventory properties
	InventoryComponent = CreateDefaultSubobject<UItemStorageComponent>("Inventory Component");
	InventoryComponent->SetStorageSize(9);
}

// Called when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Animate character on movement
	OnCharacterMovementUpdated.AddDynamic(this, &APlayerCharacter::UpdateMovementProperties);

	// Create and draw HUD
	APlayerController* Fpc = GetController<APlayerController>();
	if (IsLocallyControlled() && PlayerHUDClass && Fpc)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(Fpc, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->SetOwningPlayer(Fpc);
		PlayerHUD->AddToPlayerScreen();
		PlayerHUD->GetInventoryWidget()->SetGridPanelSizes(1, InventoryComponent->GetStorageSize());
		PlayerHUD->GetInventoryWidget()->SetCurrentInventoryType(EInventoryType::PlayerHUDInventory);
		PlayerHUD->GetInventoryWidget()->SetOwnerStorage(InventoryComponent);

		// Set up HUD for character components
		AttributesComponent->SetUpHUD(PlayerHUD);
		InventoryComponent->SetUpInventoryWidget(PlayerHUD->GetInventoryWidget());
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
	PlayerInputComponent->BindAxis("UseItem", this, &APlayerCharacter::UseItem);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::StopSprint);
	PlayerInputComponent->BindAction("Die", IE_Pressed, this, &APlayerCharacter::Die);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("ShowMouseCursor", IE_Pressed, this, &APlayerCharacter::SwitchMouseCursorVisibility);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
	
}

void APlayerCharacter::SwitchMouseCursorVisibility()
{
	APlayerController* Fpc = GetController<APlayerController>();

	if (Fpc)
	{
		if (!Fpc->bShowMouseCursor)
		{
			Fpc->bShowMouseCursor = true;
			Fpc->bEnableClickEvents = true;
			Fpc->bEnableMouseOverEvents = true;
			Fpc->SetInputMode(FInputModeGameAndUI());
			PlayerHUD->SetVisibility(ESlateVisibility::Visible);
			GetPlayerHUD()->SetCursor(EMouseCursor::Default);
		} else
		{
			Fpc->bShowMouseCursor = false;
			Fpc->bEnableClickEvents = false;
			Fpc->bEnableMouseOverEvents = false;
			Fpc->SetInputMode(FInputModeGameOnly());
			PlayerHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
			GetPlayerHUD()->SetCursor(EMouseCursor::None);
		}
	} 
}

void APlayerCharacter::UpdateMovementProperties(float DeltaTime, FVector OldLocation, FVector const OldVelocity)
{
	if (FMath::IsNearlyZero(AttributesComponent->GetStamina(), 0.1f))
	{
		StopSprint();
	} 
	
	AnimationComponent->SetCurrentCharacterDirection(OldVelocity);

	bIsMoving = !FMath::IsNearlyZero(OldVelocity.Size(), ComparisonErrorTolerance);

	if (bIsMoving && !bIsDead && !bIsAttacking)
	{
		if (GetCharacterMovement()->MaxWalkSpeed >= SprintSpeed - ComparisonErrorTolerance)
		{
			AnimationComponent->AnimateRunning();
			if (IsLocallyControlled())
			{
				AttributesComponent->UpdateStamina(RunningStaminaLossRate);
			}
		}
		else
		{
			AnimationComponent->AnimateWalking();
		}
	}
	else if (!bIsDead && !bIsAttacking)
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
		SetMaxWalkSpeed(SprintSpeed);
	}
}

// Called when shift is released
void APlayerCharacter::StopSprint()
{
	if (!bIsDead && !bIsAttacking)
	{
		SetMaxWalkSpeed(WalkSpeed);
	}
}

// Called when dying 
void APlayerCharacter::Die()
{
	if (HasLocalNetOwner())
	{
		ServerSetDying();
	}
}

// Called when client dying
void APlayerCharacter::ServerSetDying_Implementation()
{
	bIsDead = true;
	OnRep_IsDead();
}

void APlayerCharacter::Interact()
{
	if (HasLocalNetOwner())
	{
		ServerInteract();
	} 
}

void APlayerCharacter::ServerInteract_Implementation()
{
	OnRep_Interact();
}

void APlayerCharacter::OnRep_Interact_Implementation()
{
	TArray<AActor*> OverlappingActors;
	TriggerCapsule->GetOverlappingActors(OverlappingActors);


	AActor* Actor = OverlappingActors.Last();
	if (Actor && Actor != this)
	{
		IInteractableInterface* Interface = Cast<IInteractableInterface>(Actor);
		if (Interface)
		{
			UE_LOG(LogTemp, Warning, TEXT("Interacted with Actor: %s on client: %p"), *Actor->GetName(), this);
			Interface->Interact(this);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No interacted actors"));
	}
}

void APlayerCharacter::UseItem(const float Axis)
{
	if (!FMath::IsNearlyZero(Axis, ComparisonErrorTolerance) && HasLocalNetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("Use Item: %d"), FMath::RoundToInt(Axis));
		InventoryComponent->UseItem(FMath::RoundToInt(Axis) - 1);
	}
}

UItemStorageComponent* APlayerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UCharacterAttributesComponent* APlayerCharacter::GetAttributesComponent() const
{
	return AttributesComponent;
}

UPlayerHUD* APlayerCharacter::GetPlayerHUD() const
{
	return PlayerHUD;
}

AStorage* APlayerCharacter::GetInteractableStorage() const
{
	return InteractableStorage;
}

void APlayerCharacter::SetInteractableStorage(AStorage* const NewInteractableStorage)
{
	InteractableStorage = NewInteractableStorage;
}

void APlayerCharacter::ServerSetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	OnRep_SetMaxWalkSpeed(NewMaxWalkSpeed);
}

void APlayerCharacter::OnRep_SetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
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

	if (IsLocallyControlled())
	{
		if (FMath::IsNearlyZero(AttributesComponent->GetHealth(), ComparisonErrorTolerance))
		{
			Die();
		}
		
		AttributesComponent->UpdateStamina(StaminaRegenerateRate);
	}
}

// Called when some actor in overlap
void APlayerCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
									class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult)
{
	// check if Actors do not equal nullptr
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("On overlap: %s"), *OtherActor->GetName());
	}
}

void APlayerCharacter::ServerAttack_Implementation()
{
	OnRep_Attack();
}

void APlayerCharacter::OnRep_Attack_Implementation()
{
	if (!bIsAttacking && !bIsDead)
	{
		AnimationComponent->AnimateAttack();
		bIsAttacking = true;
		OnRep_SetMaxWalkSpeed(0);
	}
}

void APlayerCharacter::Attack()
{
	if (HasLocalNetOwner())
	{
		ServerAttack();
	} 
}

void APlayerCharacter::SetMaxWalkSpeed(float NewMaxWalkSpeed)
{
	if (HasLocalNetOwner())
	{
		ServerSetMaxWalkSpeed(NewMaxWalkSpeed);
	} 
}

float APlayerCharacter::GetSprintSpeed() const
{
	return SprintSpeed;
}

void APlayerCharacter::SetSprintSpeed(const float NewSprintSpeed)
{
	SprintSpeed = NewSprintSpeed;
}

float APlayerCharacter::GetWalkSpeed() const
{
	return WalkSpeed;
}

void APlayerCharacter::SetWalkSpeed(const float NewWalkSpeed)
{
	WalkSpeed = NewWalkSpeed;
}
