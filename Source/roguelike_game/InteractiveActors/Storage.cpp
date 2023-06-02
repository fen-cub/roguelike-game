// Fill out your copyright notice in the Description page of Project Settings.


#include "Storage.h"

#include "roguelike_game/Widgets/StorageDisplay.h"

AStorage::AStorage()
{
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(1.0f, 2.0f, 1.0f));

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
	Tooltip->SetAbsolute(false, true, false);
	Tooltip->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	Tooltip->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	Tooltip->SetRelativeScale3D(FVector(1.0f, 0.15f, 0.15f));
	Tooltip->SetHorizontalAlignment(EHTA_Center);
	Tooltip->SetVerticalAlignment(EVRTA_TextBottom);
	Tooltip->SetText(FText::FromString("Press E to open"));
	Tooltip->SetTextRenderColor(FColor(0, 255, 255, 255));
	Tooltip->SetHiddenInGame(true);
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
		if (PlayerCharacter->IsLocallyControlled() && Fpc && StorageWidgetClass)
		{
			StorageWidget = CreateWidget<UStorageDisplay>(Fpc, StorageWidgetClass);
			check(StorageWidget);
			StorageWidget->SetOwningPlayer(Fpc);
			StorageWidget->AddToPlayerScreen();
			StorageWidget->GetInventoryWidget()->SetGridPanelSizes(4, 5);
			StorageWidget->GetInventoryWidget()->SetCurrentInventoryType(EInventoryType::StorageInventory);
			StorageWidget->GetInventoryWidget()->SetOwnerStorage(StorageComponent);
			StorageWidget->GetInventoryWidget()->SetPairingStorage(PlayerCharacter->GetInventoryComponent());
	
			StorageComponent->SetUpInventoryWidget(StorageWidget->GetInventoryWidget());

			PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetPairingStorage(StorageComponent);
			PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetCurrentInventoryType(
				EInventoryType::PlayerInventoryInStorage);
			PlayerCharacter->SetInteractableStorage(this);
		}
	}
}

void AStorage::StopInteract(APlayerCharacter* PlayerCharacter)
{
	if (StorageWidget && PlayerCharacter->IsLocallyControlled())
	{
		StorageWidget->RemoveFromParent();
		StorageWidget->Destruct();

		PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetPairingStorage(nullptr);
		PlayerCharacter->GetPlayerHUD()->GetInventoryWidget()->SetCurrentInventoryType(
			EInventoryType::PlayerHUDInventory);
		PlayerCharacter->SetInteractableStorage(nullptr);
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