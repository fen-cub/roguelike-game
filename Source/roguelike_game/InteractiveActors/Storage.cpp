// Fill out your copyright notice in the Description page of Project Settings.


#include "Storage.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/Widgets/EquipmentWidget.h"
#include "roguelike_game/Widgets/StorageDisplay.h"

AStorage::AStorage()
{
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(15.0f, 15.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AStorage::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AStorage::OnOverlapEnd);

	StorageComponent = CreateDefaultSubobject<UItemStorageComponent>("Inventory Component");
	StorageComponent->SetStorageSize(20);

	Tooltip = CreateDefaultSubobject<class UTextRenderComponent>("Tooltip");
	Tooltip->SetupAttachment(RootComponent);
	Tooltip->SetAbsolute(false, true, true);
	Tooltip->SetRelativeLocation(FVector(0.0f, 200.0f, 10.0f));
	Tooltip->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	Tooltip->SetWorldScale3D(FVector(1.0f, 0.15f, 0.15f));
	Tooltip->SetHorizontalAlignment(EHTA_Center);
	Tooltip->SetVerticalAlignment(EVRTA_TextBottom);
	Tooltip->SetText(FText::FromString("Press E to open"));
	Tooltip->SetTextRenderColor(FColor(255, 122, 0, 255));
	Tooltip->SetHiddenInGame(true);
}

void AStorage::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		SetOwner(PlayerController);
	}
}

void AStorage::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
							class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(false);
	}
}

void AStorage::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
							class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(true);
	}
}

UItemStorageComponent* AStorage::GetStorageComponent() const
{
	return StorageComponent;
}

void AStorage::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		APlayerController* Fpc = PlayerCharacter->GetController<APlayerController>();
		SetOwner(Fpc);

		if (PlayerCharacter->IsLocallyControlled() && Fpc && StorageWidgetClass && PlayerCharacter->GetPlayerHUD())
		{
			StorageWidget = CreateWidget<UStorageDisplay>(Fpc, StorageWidgetClass);
			check(StorageWidget);
			PlayerCharacter->GetPlayerHUD()->SetInteractableStorageWidget(StorageWidget);
			StorageWidget->SetOwningPlayer(Fpc);
			StorageWidget->AddToPlayerScreen();
			StorageWidget->GetInventoryWidget()->SetGridPanelSizes(4, 5);
			StorageWidget->GetInventoryWidget()->SetCurrentInventoryType(EInventoryType::StorageInventory);
			StorageWidget->GetInventoryWidget()->SetOwnerStorage(StorageComponent);
			StorageWidget->GetInventoryWidget()->SetPairingStorage(PlayerCharacter->GetInventoryComponent());
			StorageComponent->SetUpInventoryWidget(StorageWidget->GetInventoryWidget());
			StorageWidget->SetFocus();

			PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetPairingStorage(StorageComponent);
			PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetCurrentInventoryType(
				EInventoryType::PlayerInventoryInStorage);
			PlayerCharacter->SetMaxWalkSpeed(0);

			Fpc->SetInputMode(FInputModeUIOnly());
			Fpc->SetShowMouseCursor(true);
			StorageWidget->SetCursor(EMouseCursor::Type::Default);
			PlayerCharacter->GetPlayerHUD()->SetCursor(EMouseCursor::Type::Default);
			PlayerCharacter->GetPlayerHUD()->SetVisibility(ESlateVisibility::Visible);
			PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->HideLastClickedSlot();
		}

		PlayerCharacter->SetInteractableStorage(this);
		UGameplayStatics::SpawnSoundAtLocation(PlayerCharacter, InteractSound, PlayerCharacter->GetActorLocation());
	}
}

void AStorage::StopInteract(APlayerCharacter* PlayerCharacter)
{
	PlayerCharacter->SetInteractableStorage(nullptr);


	if (StorageWidget && PlayerCharacter->IsLocallyControlled())
	{
		StorageWidget->RemoveFromParent();
		StorageWidget->Destruct();

		StorageWidget->GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
		PlayerCharacter->SetMaxWalkSpeed(PlayerCharacter->GetWalkSpeed());
		PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetPairingStorage(nullptr);
		PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetCurrentInventoryType(
			EInventoryType::PlayerHUDInventory);

		StorageWidget->GetOwningPlayer()->SetShowMouseCursor(false);
		StorageWidget->SetCursor(EMouseCursor::Type::None);
		PlayerCharacter->GetPlayerHUD()->SetCursor(EMouseCursor::Type::None);

		PlayerCharacter->GetPlayerHUD()->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->HideLastClickedSlot();
		PlayerCharacter->GetPlayerHUD()->GetEquipmentWidget()->HideLastClickedSlot();
	}

	UGameplayStatics::SpawnSoundAtLocation(PlayerCharacter, StopInteractSound, PlayerCharacter->GetActorLocation());
}
