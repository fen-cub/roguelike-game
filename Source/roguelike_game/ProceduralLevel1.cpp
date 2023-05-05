// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLevel1.h"
#include "PaperTileSet.h"

// Sets default values
AProceduralLevel1::AProceduralLevel1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UClass* ClassA = ARoomActor::StaticClass();
	RoomActorClass = ClassA;
	NumOfRooms = 6;
}


void AProceduralLevel1::BeginPlay()
{
	Super::BeginPlay();

	uint8 RoomsExist = 0;

	TQueue<ARoomActor*> RoomQueue;

	ARoomActor* MyRoom = CreateRoomWithDoors();
	RoomsExist++;

	RoomQueue.Enqueue(MyRoom);
	UE_LOG(LogTemp, Warning, TEXT("Add in queue"))

	while (!RoomQueue.IsEmpty() && RoomsExist <= NumOfRooms)
	{
		ARoomActor* FromQueue = nullptr;
		RoomQueue.Peek(FromQueue);
		RoomQueue.Pop();
		UE_LOG(LogTemp, Warning, TEXT("From queue"))
		for (uint8 i = 0; i < 4; i++)
		{
			if (FromQueue->RoomComponent->Doors[i])
			{
				const FVector PrevLocation = FromQueue->GetActorLocation();
				ARoomActor* NewRoom = CreateRoomWithDir((i + 2) % 4, PrevLocation);
				RoomsExist++;
				RoomQueue.Enqueue(NewRoom);
				FromQueue->RoomComponent->Doors[i] = false;
			}
		}
	}
	
	while(!RoomQueue.IsEmpty()) {
		UPaperTileSet *TileSet = LoadObject<UPaperTileSet>(
	nullptr, TEXT("/Game/Textures/TX_Tileset_Grass_TileSet"));
		FPaperTileInfo TileInfo;
		TileInfo.TileSet = TileSet;
		TileInfo.PackedTileIndex = 32;
		ARoomActor* FromQueue = nullptr;
		RoomQueue.Peek(FromQueue);
		RoomQueue.Pop();
		for (uint8 i = 0; i < 4; i++)
		{
			if (FromQueue->RoomComponent->Doors[i])
			{
				switch (i) {
				case 0:
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 - 2, 0, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 - 1, 0, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2, 0, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 + 1, 0, 0, TileInfo);
					break;
				case 1:
					FromQueue->RoomComponent->SetTile(0, FromQueue->RoomComponent->RoomHeight / 2 - 2, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(0, FromQueue->RoomComponent->RoomHeight / 2 - 1, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(0, FromQueue->RoomComponent->RoomHeight / 2, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(0, FromQueue->RoomComponent->RoomHeight / 2 + 1, 0, TileInfo);
					break;
				case 2:
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 - 2, FromQueue->RoomComponent->RoomHeight - 1, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 - 1, FromQueue->RoomComponent->RoomHeight - 1, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2, FromQueue->RoomComponent->RoomHeight - 1, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth / 2 + 1, FromQueue->RoomComponent->RoomHeight - 1, 0, TileInfo);
					break;
				default:
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth - 1, FromQueue->RoomComponent->RoomHeight / 2 - 2, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth - 1, FromQueue->RoomComponent->RoomHeight / 2 - 1, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth - 1, FromQueue->RoomComponent->RoomHeight / 2, 0, TileInfo);
					FromQueue->RoomComponent->SetTile(FromQueue->RoomComponent->RoomWidth - 1, FromQueue->RoomComponent->RoomHeight / 2 + 1, 0, TileInfo);
					break;
				}
			}
		}
		FromQueue->RoomComponent->SetLayerCollision(0, true, true, 100);
	}

}



// Called every frame
void AProceduralLevel1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ARoomActor* AProceduralLevel1::CreateRoomWithDoors() const
{
	ARoomActor* MyRoom;
	const int32 MaxAttempts = 5;
	int32 Attempts = 0;

	while (true)
	{
		ARoomActor* FirstRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, FVector::ZeroVector, FRotator(0.f, 90.f, -90.f));
		UE_LOG(LogTemp, Warning, TEXT("Create First Room"))
		if(Attempts	>= MaxAttempts)
		{
			MyRoom = FirstRoom;
			break;
		}

		if (FirstRoom->RoomComponent->Doors[0] == false && FirstRoom->RoomComponent->Doors[1] == false && FirstRoom->RoomComponent->Doors[2] == false && FirstRoom->RoomComponent->Doors[3] == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad Room"))	
			FirstRoom->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Bad Room Destroyed"))
			Attempts++;
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Good Room"))
			MyRoom = FirstRoom;
			break;
		}
	}
	
	return MyRoom;
}

ARoomActor* AProceduralLevel1::CreateRoomWithDir(const uint8 Direction, FVector PrevLocation) const
{
	float NewX, NewY;
	const int32 MaxAttempts = 5;
	int32 Attempts = 0;
	switch (Direction)
	{
	case 0:
		NewX = -320.f;
		NewY = 0.f;
		break;
	case 1:
		NewX = 0.f;
		NewY = 320.f;
		break;
	case 2:
		NewX = 320.f;
		NewY = 0.f;
		break;
	default:
		NewX = 0.f;
		NewY = -320.f;
		break;
	}
	FVector NewLocation = FVector(PrevLocation.X + NewX, PrevLocation.Y + NewY, PrevLocation.Z);
	ARoomActor* MyRoom;

	while (true)
	{
		ARoomActor* FirstRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewLocation, FRotator(0.f, 90.f, -90.f));
		UE_LOG(LogTemp, Warning, TEXT("Create Dir Room"))
		if(Attempts	>= MaxAttempts)
		{
			MyRoom = FirstRoom;
			break;
		}

		if (FirstRoom->RoomComponent->Doors[Direction] == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad Room"))	
			FirstRoom->Destroy();
			UE_LOG(LogTemp, Warning, TEXT("Bad Room Destroyed"))
			Attempts++;
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Good Room"))
			MyRoom = FirstRoom;
			break;
		}
	}

	MyRoom->RoomComponent->Doors[Direction] = false;
	
	return MyRoom;
}


