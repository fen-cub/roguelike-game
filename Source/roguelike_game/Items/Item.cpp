// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/ItemStorageComponent.h"


bool FItemData::IsEmpty() const
{
	return Image == nullptr;
}

AItem::AItem()
{
	bReplicates = true;
	SetReplicateMovement(true);

	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(20.0f, 20.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
	TriggerCapsule->SetIsReplicated(true);

	Tooltip = CreateDefaultSubobject<class UTextRenderComponent>("Tooltip");
	Tooltip->SetupAttachment(RootComponent);
	Tooltip->SetAbsolute(false, true, true);
	Tooltip->SetRelativeLocation(FVector(0.0f, 200.0f, 10.0f));
	Tooltip->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	Tooltip->SetWorldScale3D(FVector(1.0f, 0.15f, 0.15f));
	Tooltip->SetHorizontalAlignment(EHTA_Center);
	Tooltip->SetVerticalAlignment(EVRTA_TextBottom);
	Tooltip->SetText(FText::FromString("Press E to take"));
	Tooltip->SetTextRenderColor(FColor(255, 122, 0, 255));
	Tooltip->SetHiddenInGame(true);
	Tooltip->SetIsReplicated(true);

	GetRenderComponent()->SetMobility(EComponentMobility::Movable);
	GetRenderComponent()->SetSimulatePhysics(true);
	GetRenderComponent()->SetEnableGravity(true);
	GetRenderComponent()->SetConstraintMode(EDOFMode::Type::Default);
	GetRenderComponent()->GetBodyInstance()->bLockXRotation = true;
	GetRenderComponent()->GetBodyInstance()->bLockYRotation = true;
	GetRenderComponent()->GetBodyInstance()->MassScale = 500.0f;
	GetRenderComponent()->CanCharacterStepUpOn = ECB_No;
	GetRenderComponent()->SetIsReplicated(true);

	Data.Name = GetName();
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AItem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
							class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(false);
	}
}

void AItem::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(true);
	}
}

void AItem::Interact(class APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		const int64 Position = PlayerCharacter->GetInventoryComponent()->GetFirstEmptySlotPosition();

		if (Position < PlayerCharacter->GetInventoryComponent()->GetStorageSize())
		{
			PlayerCharacter->GetInventoryComponent()->AddItem(GetItemData(),
															Position);
			UGameplayStatics::SpawnSoundAtLocation(this, InteractSound,  PlayerCharacter->GetActorLocation());
			Destroy();
		}
	}
}

FItemData AItem::GetItemData() const
{
	return Data;
}

void AItem::Use(APlayerCharacter* PlayerCharacter, int64 InventoryPosition)
{
}

void AItem::SetItemData(const FItemData& NewData)
{
	Data = NewData;
}
