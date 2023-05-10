// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Net/UnrealNetwork.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"


bool FItemData::IsEmpty() const
{
	return Image == nullptr;
}

AItem::AItem()
{
	SetReplicates(true);
	
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(1.0f, 2.0f, 1.0f));

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(15.0f, 15.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

	Tooltip = CreateDefaultSubobject<class UTextRenderComponent>("Tooltip");
	Tooltip->SetupAttachment(RootComponent);
	Tooltip->SetAbsolute(false, true, false);
	Tooltip->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	Tooltip->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	Tooltip->SetRelativeScale3D(FVector(1.0f, 0.15f, 0.15f));
	Tooltip->SetHorizontalAlignment(EHTA_Center);
	Tooltip->SetVerticalAlignment(EVRTA_TextBottom);
	Tooltip->SetText(FText::FromString("Press E to take item"));
	Tooltip->SetTextRenderColor(FColor(0, 255, 255, 255));
	Tooltip->SetHiddenInGame(true);

	GetRenderComponent()->SetMobility(EComponentMobility::Movable);
	GetRenderComponent()->SetSimulatePhysics(true);
	GetRenderComponent()->SetEnableGravity(true);
	GetRenderComponent()->SetConstraintMode(EDOFMode::Type::Default);
	GetRenderComponent()->GetBodyInstance()->bLockXRotation = true;
	GetRenderComponent()->GetBodyInstance()->bLockYRotation = true;
	GetRenderComponent()->GetBodyInstance()->MassScale = 50.0f;
	GetRenderComponent()->CanCharacterStepUpOn = ECB_No;
}

void AItem::Interact(class APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		PlayerCharacter->Inventory->AddItem(GetItemData());
		Destroy();
	}
}

FItemData AItem::GetItemData() const
{
	return Data;
}

void AItem::Use(APlayerCharacter* PlayerCharacter)
{
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
