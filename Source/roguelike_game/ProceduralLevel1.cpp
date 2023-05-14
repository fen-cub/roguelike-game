// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLevel1.h"


// Sets default values
AProceduralLevel1::AProceduralLevel1()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UClass* ClassA = ARoomActor::StaticClass();
	RoomActorClass = ClassA;
}

int Conjugate(const int X)
{
	return (X + 2) % 4;
}

void AProceduralLevel1::BeginPlay()
{
	Super::BeginPlay();

	NumOfRooms = FMath::RandRange(0, 1) + 5 + LevelNumber * 2.6;
	uint8 RoomsExist = 0;

	TQueue<TPair<uint8, uint8>> RoomQueue;
	TArray<ARoomActor*> EndRooms;
	TQueue<ARoomActor*> AllRooms;

	while (true)
	{
		const TPair<uint8, uint8> StartRoom(MapWidth / 2, MapHeight / 2);

		RoomQueue.Enqueue(StartRoom);
		RoomsExist++;
		UE_LOG(LogTemp, Warning, TEXT("Add in queue"))

		while (RoomQueue.IsEmpty() != true)
		{
			TPair<uint8, uint8> FromQueue;
			RoomQueue.Peek(FromQueue);
			LevelMap[FromQueue.Key][FromQueue.Value].IsInQueue = false;
			RoomQueue.Pop();
			for (uint8 i = 0; i < 4; i++) {
				if (FromQueue.Key + DirX[i] < 0 || FromQueue.Key + DirX[i] >= MapWidth || FromQueue.Value + DirY[i] < 0 || FromQueue.Value + DirY[i] >= MapHeight)
				{
					continue;
				}
				if (LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].Doors.Num() != 0) {
					continue;
				}
				if (RoomsExist == NumOfRooms)
				{
					continue;
				}
				if (FMath::RandRange(0, 2) == 0)
				{
					continue;
				}
				RoomsExist++;
				LevelMap[FromQueue.Key][FromQueue.Value].Doors.Add(i);
				LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].Doors.Add(Conjugate(i));
				if (LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].IsInQueue == false)
				{
					RoomQueue.Enqueue(TPair<int, int>(FromQueue.Key + DirX[i],FromQueue.Value + DirY[i]));
					LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].IsInQueue = true;
				}
			}
			if (LevelMap[FromQueue.Key][FromQueue.Value].Doors.Num() != 0)
			{
				ARoomActor* NewRoom = CreateRoom(StartRoom, FromQueue, AllRooms);
				if (LevelMap[FromQueue.Key][FromQueue.Value].Doors.Num() == 1)
				{
					EndRooms.Add(NewRoom);
				}
			}
			LevelMap[FromQueue.Key][FromQueue.Value].WasInQueue = true;
		}

		if (RoomsExist == NumOfRooms)
		{
			break;
		} 
		UE_LOG(LogTemp, Warning, TEXT("Bad Room Set"))
		while(AllRooms.IsEmpty() != true)
		{
			ARoomActor* RoomToDelete;
			AllRooms.Peek(RoomToDelete);
			AllRooms.Pop();
			RoomToDelete->Destroy();
		}
		for (int i = 0; i < MapWidth; i++)
		{
			for (int j = 0; j < MapHeight; j++)
			{
				LevelMap[i][j].Doors.Empty();
				LevelMap[i][j].IsInQueue = false;
				LevelMap[i][j].WasInQueue = false;
			}
		}
		RoomsExist = 0;
	}
}

ARoomActor* AProceduralLevel1::CreateRoom(const TPair<int, int> StartRoom, const TPair<int, int> CurrentRoom, TQueue<ARoomActor*> &AllRooms)
{
	const FVector NewRoomLocation = FVector((StartRoom.Value - CurrentRoom.Value) * (RealRoomWidth + RealTileHeight * CorridorHeight), (StartRoom.Key - CurrentRoom.Key) * (RealRoomHeight + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f));
	NewRoom->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight);
	AllRooms.Enqueue(NewRoom);
	for (const int& x : LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors)
	{
		if (LevelMap[CurrentRoom.Key + DirX[x]][CurrentRoom.Value + DirY[x]].WasInQueue == false) {
			FVector NewCorridorLocation = FVector((StartRoom.Value - CurrentRoom.Value) * (RealRoomWidth + CorridorHeight * RealTileHeight) + CorrDirX[x], (StartRoom.Key - CurrentRoom.Key) * (RealRoomHeight + CorridorHeight * RealTileHeight) + CorrDirY[x], 0.f);
			ARoomActor* NewCorridor = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewCorridorLocation, FRotator(0.f, 90.f, -90.f));
			TSet<int> CorridorDir;
			if (x % 2 == 0)
			{
				CorridorDir.Add(0);
				CorridorDir.Add(2);
				NewCorridor->Init(CorridorDir, CorridorWidth, CorridorHeight);
			} else
			{
				CorridorDir.Add(1);
				CorridorDir.Add(3);
				NewCorridor->Init(CorridorDir, CorridorHeight, CorridorWidth);
			}
			AllRooms.Enqueue(NewCorridor);
		}
	}
	return NewRoom;
}



// Called every frame
void AProceduralLevel1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

