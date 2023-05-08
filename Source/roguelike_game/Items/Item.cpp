// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"


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

	Sprite->SetRelativeRotation(FRotator(0.0f, 90.0f, -90.0f));
	Sprite->SetFlipbook(ItemFlipbook);

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(10.0f, 10.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
}

void AItem::Interact()
{
	Destroy();
}
