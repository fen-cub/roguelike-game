// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "roguelike_game/Components/CharacterAnimationComponent.h"
#include "roguelike_game/Components/CharacterAttributesComponent.h"
#include "roguelike_game/Items/Item.h"
#include "Components/InputComponent.h"
#include "roguelike_game/Components/ItemStorageComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/InteractiveActors/Storage.h"
#include "roguelike_game/Widgets/EquipmentWidget.h"

// Set default player properties
APlayerCharacter::APlayerCharacter()
{
	// Default game logic properties
	bIsDead = false;
	bIsMoving = false;
	PrimaryActorTick.bCanEverTick = true;
	ComparisonErrorTolerance = 1e-4f;
	StaminaRegenerateRate = 0.15f;
	RunningStaminaLossRate = -0.5f;
	HealthRegenerateRate = 0.01f;

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
	GetCapsuleComponent()->InitCapsuleSize(11.0f, 11.0f);

	// Default sprite properties
	GetSprite()->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	GetSprite()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetSprite()->SetRelativeLocation(FVector(9.0f, 0.0f, 0.0f));

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
	TriggerCapsule->InitCapsuleSize(15.0f, 15.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);

	// Default inventory properties
	InventoryComponent = CreateDefaultSubobject<UItemStorageComponent>("Inventory Component");
	InventoryComponent->SetStorageSize(9);

	// Default inventory properties
	EquipmentComponent = CreateDefaultSubobject<UItemStorageComponent>("Equipment Component");
	EquipmentComponent->SetStorageSize(3);
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

		PlayerHUD->GetEquipmentWidget()->SetGridPanelSizes(3, 1);
		PlayerHUD->GetEquipmentWidget()->SetCurrentInventoryType(EInventoryType::EquipmentInventory);
		PlayerHUD->GetEquipmentWidget()->SetOwnerStorage(EquipmentComponent);

		// Set up HUD for character components
		AttributesComponent->SetUpHUD(PlayerHUD);
		InventoryComponent->SetUpInventoryWidget(PlayerHUD->GetInventoryWidget());
		EquipmentComponent->SetUpInventoryWidget(PlayerHUD->GetEquipmentWidget());
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
	DOREPLIFETIME(APlayerCharacter, WalkSpeed);
	DOREPLIFETIME(APlayerCharacter, SprintSpeed);
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
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("ShowMouseCursor", IE_Pressed, this,
									&APlayerCharacter::SwitchMouseCursorVisibility);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);

	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem1", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(1));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem2", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(2));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem3", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(3));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem4", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(4));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem5", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(5));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem6", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(6));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem7", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(7));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem8", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(8));
	PlayerInputComponent->BindAction<FNumberKeyActionDelegate>("UseItem9", IE_Pressed, this, &APlayerCharacter::UseItem,
																static_cast<int64>(9));
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
			if (PlayerHUD)
			{
				PlayerHUD->SetVisibility(ESlateVisibility::Visible);
				PlayerHUD->SetCursor(EMouseCursor::Default);
			}
		}
		else
		{
			Fpc->bShowMouseCursor = false;
			Fpc->bEnableClickEvents = false;
			Fpc->bEnableMouseOverEvents = false;
			Fpc->SetInputMode(FInputModeGameOnly());
			if (PlayerHUD)
			{
				PlayerHUD->SetVisibility(ESlateVisibility::HitTestInvisible);
				PlayerHUD->SetCursor(EMouseCursor::None);
				PlayerHUD->GetInventoryWidget()->HideLastClickedSlot();
				PlayerHUD->GetEquipmentWidget()->HideLastClickedSlot();
			}
		}
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
		// Actor on overlap...
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
	if (!FMath::IsNearlyZero(AttributesComponent->GetStamina(), ComparisonErrorTolerance) && !bIsDead && !bIsAttacking)
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

void APlayerCharacter::UseItem(const int64 Position)
{
	if (!bIsDead && HasLocalNetOwner())
	{
		InventoryComponent->UseItem(Position - 1);
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

void APlayerCharacter::ServerSetDying_Implementation()
{
	bIsDead = true;
	OnRepSetDying();
}

// Calls back from server when dying
void APlayerCharacter::OnRepSetDying()
{
	AnimationComponent->AnimateDeath();

	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}

	// EndPlay(EEndPlayReason::Destroyed);
}

void APlayerCharacter::Interact()
{
	if (!bIsDead && HasLocalNetOwner())
	{
		ServerInteract();
	}
}

void APlayerCharacter::ServerInteract_Implementation()
{
	OnRep_Interact(FMath::RandRange(0LL, (1LL << 32LL)));
}

void APlayerCharacter::OnRep_Interact_Implementation(int64 RandomHash)
{
	TArray<AActor*> OverlappingActors;
	TriggerCapsule->GetOverlappingActors(OverlappingActors);


	AActor* Actor = OverlappingActors.Last();
	if (Actor && Actor != this)
	{
		IInteractableInterface* Interface = Cast<IInteractableInterface>(Actor);
		if (Interface)
		{
			AStorage* Storage = Cast<AStorage>(Actor);
			if (Storage && !Storage->GetStorageComponent()->bIsGenerated)
			{
				Storage->GetStorageComponent()->GenerateRandomContents(RandomHash);
			}

			Interface->Interact(this);
		}
	}
}

void APlayerCharacter::Attack()
{
	if (HasLocalNetOwner())
	{
		ServerAttack();
	}
}

void APlayerCharacter::ServerAttack_Implementation()
{
	OnRep_Attack();
}

void APlayerCharacter::OnRep_Attack_Implementation()
{
	if (!bIsAttacking && !bIsDead && AttributesComponent->GetStamina() >= 5.0f - ComparisonErrorTolerance)
	{
		AnimationComponent->AnimateAttack();
		bIsAttacking = true;
		SetMaxWalkSpeed(0);

		AttributesComponent->UpdateStamina(-5.0f);
	}
}

void APlayerCharacter::ServerSetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	OnRep_SetMaxWalkSpeed(NewMaxWalkSpeed);
}

void APlayerCharacter::OnRep_SetMaxWalkSpeed_Implementation(float NewMaxWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxWalkSpeed;
}

void APlayerCharacter::ServerSetWalkSpeed_Implementation(float NewWalkSpeed)
{
	WalkSpeed = NewWalkSpeed;
}

void APlayerCharacter::ServerSetSprintSpeed_Implementation(float NewSprintSpeed)
{
	SprintSpeed = NewSprintSpeed;
}

UItemStorageComponent* APlayerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UItemStorageComponent* APlayerCharacter::GetEquipmentComponent() const
{
	return EquipmentComponent;
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
	if (HasLocalNetOwner())
	{
		ServerSetSprintSpeed(NewSprintSpeed);
	}
}

float APlayerCharacter::GetWalkSpeed() const
{
	return WalkSpeed;
}

void APlayerCharacter::SetWalkSpeed(const float NewWalkSpeed)
{
	if (HasLocalNetOwner())
	{
		ServerSetWalkSpeed(NewWalkSpeed);
	}
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
		AttributesComponent->UpdateHealth(HealthRegenerateRate);
	}
}
