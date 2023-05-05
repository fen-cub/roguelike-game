// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"


// Sets default values
ARoomActor::ARoomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoomComponent = CreateDefaultSubobject<URoom>(TEXT("RoomComponent"));
	TArray<bool> Doors;
	for (uint8 j = 0; j < 4; j++)
	{
		const uint8 HasADoor = FMath::FRandRange(0, 2);
		Doors.Add(HasADoor == 1 ? true : false);
	}
	RoomComponent->SetDoors(Doors);
	RoomComponent->CreateRoom(20, 20);
	SetRootComponent(RoomComponent);
}

// Called when the game starts or when spawned
void ARoomActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARoomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

