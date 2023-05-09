// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Character/Components/ItemStorageComponent.h"


bool FItemData::IsEmpty() const
{
	return Image == nullptr;
}

AItem::AItem()
{
	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>("SpriteComponent");
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = true;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		static FName CollisionProfileName(TEXT("CharacterMesh"));
		Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);
	}
	
	Sprite->SetFlipbook(ItemFlipbook);
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(10.0f, 10.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
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
