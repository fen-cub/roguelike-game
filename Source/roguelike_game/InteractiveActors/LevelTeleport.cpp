// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTeleport.h"

#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "roguelike_game/TestGameState.h"
#include "roguelike_game/Character/PlayerCharacter.h"


ALevelTeleport::ALevelTeleport()
{
	SetActorRotation(FRotator(0.0f, 90.0f, -90.0f));
	SetActorRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	TriggerCapsule = CreateDefaultSubobject<class UCapsuleComponent>("Trigger capsule");
	TriggerCapsule->InitCapsuleSize(15.0f, 15.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ALevelTeleport::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ALevelTeleport::OnOverlapEnd);

	Tooltip = CreateDefaultSubobject<class UTextRenderComponent>("Tooltip");
	Tooltip->SetupAttachment(RootComponent);
	Tooltip->SetAbsolute(false, true, true);
	Tooltip->SetRelativeLocation(FVector(0.0f, 200.0f, 10.0f));
	Tooltip->SetRelativeRotation(FRotator(90.0f, 180.0f, 0.0f));
	Tooltip->SetWorldScale3D(FVector(1.0f, 0.15f, 0.15f));
	Tooltip->SetHorizontalAlignment(EHTA_Center);
	Tooltip->SetVerticalAlignment(EVRTA_TextBottom);
	Tooltip->SetText(FText::FromString("Press E to go next level"));
	Tooltip->SetTextRenderColor(FColor(255, 122, 0, 255));
	Tooltip->SetHiddenInGame(true);
}

void ALevelTeleport::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

		SetOwner(PlayerController);
	}
}

void ALevelTeleport::Interact(APlayerCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		APlayerController* Fpc = PlayerCharacter->GetController<APlayerController>();
		SetOwner(Fpc);

		if (PlayerCharacter->IsLocallyControlled() && Fpc)
		{
			GetWorld()->GetGameState<ATestGameState>()->OpenNextLevel();
			// Implement logic
		}
	}
}

void ALevelTeleport::StopInteract(APlayerCharacter* PlayerCharacter)
{
}

void ALevelTeleport::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
									class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(false);
	}
}

void ALevelTeleport::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
								class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this && Cast<APlayerCharacter>(OtherActor))
	{
		Tooltip->SetHiddenInGame(true);
	}
}
