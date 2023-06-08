// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomActor.h"


// Sets default values
ARoomActor::ARoomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RoomComponent = CreateDefaultSubobject<URoom>(TEXT("RoomComponent"));
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

void ARoomActor::Init(TSet<int> Doors, const int Width, const int Height, TSet<int> Walls, const uint8 Side, const int TemplateNum)
{
	TArray<bool> BDoors;
	TArray<bool> BWalls;
	BWalls.Init(true, 4);
	BDoors.Init(false, 4);
	for (const int &x : Doors)
	{
		BDoors[x] = true;
	}
	for (const int &x : Walls)
	{
		BWalls[x] = false;
	}
	RoomComponent->Doors = BDoors;
	RoomComponent->Walls = BWalls;
	RoomComponent->CreateRoom(Width, Height, Side, TemplateNum);
	SetRootComponent(RoomComponent);
}

