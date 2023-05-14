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

void ARoomActor::Init(TSet<int> Doors, int Width, int Height)
{
	TArray<bool> BDoors;
	BDoors.Init(false, 4);
	for (const int &x : Doors)
	{
		BDoors[x] = true;
	}
	RoomComponent->SetDoors(BDoors);
	RoomComponent->CreateRoom(Width, Height);
	SetRootComponent(RoomComponent);
}

