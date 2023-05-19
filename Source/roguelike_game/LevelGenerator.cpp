// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"


// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UClass* ClassA = ARoomActor::StaticClass();
	RoomActorClass = ClassA;
}

// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int Conjugate(const int X)
{
	return (X + 2) % 4;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	NumOfRooms = FMath::RandRange(0, 1) + 5 + LevelNumber * 2.6;
	MaxBigRooms = LevelNumber;
	MaxLongRooms = LevelNumber * 1.5;
	MaxLTypeRooms = LevelNumber * 1.5;
	
	uint8 RoomsExist = 0;
	uint8 BigRoomsExist = 0;
	uint8 LongRoomsExist = 0;
	uint8 LTypeRoomsExist = 0;

	while (true) {
		LevelMap[FirstRoom.Key][FirstRoom.Value].CellRoomType = Default;
		LevelMap[FirstRoom.Key][FirstRoom.Value].Main = true;
		RoomQueue.Enqueue(FirstRoom);
		RoomsExist++;

		while (RoomQueue.IsEmpty() != true) {
			TPair<uint8, uint8> FromQueue;
			RoomQueue.Peek(FromQueue);
			auto &CurrentCell = LevelMap[FromQueue.Key][FromQueue.Value];
			RoomQueue.Pop();
			for (uint8 i = 0; i < 4; i++) {
				auto &NeighboringCell = LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]];
				if (FromQueue.Key + DirX[i] < 0 || FromQueue.Key + DirX[i] >= MapWidth || FromQueue.Value + DirY[i] < 0 || FromQueue.Value + DirY[i] >= MapHeight)
				{
					continue;
				}
				if (NeighboringCell.CellRoomType != None) {
					continue;
				}
				if (RoomsExist == NumOfRooms)
				{
					continue;
				}
				if (LongRoomsExist < MaxLongRooms)
				{
					const uint8 LongRoom = CanAddLongRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i);
					if (LongRoom)
					{
						RoomsExist++;
						LongRoomsExist++;
						CurrentCell.Doors.Add(i);
						NeighboringCell.Doors.Add(Conjugate(i));
						SetLongRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), LongRoom);
					}
				} else if (FMath::RandBool())
				{
					RoomsExist++;
					CurrentCell.Doors.Add(i);
					NeighboringCell.Doors.Add(Conjugate(i));
					NeighboringCell.CellRoomType = Default;
					NeighboringCell.Main = true;
					RoomQueue.Enqueue(TPair<int, int>(FromQueue.Key + DirX[i],FromQueue.Value + DirY[i]));
				}
			}
			if (CurrentCell.Main)
			{
				if (CurrentCell.CellRoomType == Default)
				{
					CreateDefaultRoom(FromQueue);
					CurrentCell.Generated = true;
				}
				
				if (CurrentCell.CellRoomType == Long)
				{
					CreateLongRoom(FromQueue, LevelMap[FromQueue.Key][FromQueue.Value].Direction);
				}
				
			}
		}

		if (RoomsExist != NumOfRooms)
		{
			Clear();
			RoomsExist = 0;
			LongRoomsExist = 0;
			LTypeRoomsExist = 0;
			BigRoomsExist = 0;
		} else
		{
			break;
		}
	}
}

void ALevelGenerator::CreateDefaultRoom(const TPair<uint8, uint8> CurrentRoom)
{
	TSet<int> EmptySet;
	const FVector NewRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f));
	NewRoom->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, EmptySet);
	AllRooms.Enqueue(NewRoom);
	CreateCorridors(CurrentRoom);
}


int ALevelGenerator::CanAddLongRoom(const TPair<uint8, uint8> StartRoom, uint8 Dir) const
{
	for (int8 i = -1; i <= 1; i++)
	{
		if (StartRoom.Key + DirX[(Dir + i + 4) % 4] >= 0 && StartRoom.Key + DirX[(Dir + i + 4) % 4] < MapWidth && StartRoom.Value + DirY[(Dir + i + 4) % 4] >= 0 && StartRoom.Value + DirY[(Dir + i + 4) % 4] < MapHeight) {
			if (LevelMap[StartRoom.Key + DirX[(Dir + i + 4) % 4]][StartRoom.Value + DirY[(Dir + i + 4) % 4]].CellRoomType == None && FMath::RandBool())
			{
				return (Dir + i + 4) % 4 + 2;
			}
		}
	}
	return 0;
}

void ALevelGenerator::SetLongRoom(const TPair<int, int> CurrentRoom, int Dir)
{
	Dir -= 2;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].CellRoomType = Long;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Main = true;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Direction = Dir;
	LevelMap[CurrentRoom.Key + DirX[Dir]][CurrentRoom.Value + DirY[Dir]].CellRoomType = Long;
	RoomQueue.Enqueue(TPair<int, int>(CurrentRoom.Key + DirX[Dir], CurrentRoom.Value + DirY[Dir]));
	RoomQueue.Enqueue(CurrentRoom);
}

void ALevelGenerator::CreateLongRoom(const TPair<int, int> CurrentRoom, int Dir)
{
	TSet<int> EmptySet;
	const TPair<int, int> SecondRoom = TPair<int, int>(CurrentRoom.Key + DirX[Dir], CurrentRoom.Value + DirY[Dir]);
	const FVector CurRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom1 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, CurRoomLocation, FRotator(0.f, 90.f, -90.f));
	TSet<int> CurRoomWalls;
	CurRoomWalls.Add(Dir);
	NewRoom1->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, CurRoomWalls);
	CreateCorridors(CurrentRoom);
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
	
	
	const FVector SecondRoomLocation = FVector((FirstRoom.Value - SecondRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - SecondRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom2 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, SecondRoomLocation, FRotator(0.f, 90.f, -90.f));
	TSet<int> SecondRoomWalls;
	SecondRoomWalls.Add(Conjugate(Dir));
	NewRoom2->Init(LevelMap[SecondRoom.Key][SecondRoom.Value].Doors, RoomWidth, RoomHeight, SecondRoomWalls);
	CreateCorridors(SecondRoom);
	LevelMap[SecondRoom.Key][SecondRoom.Value].Generated = true;

	
	const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight) + AddDirX[Dir], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight) + AddDirY[Dir], 0.f);
	ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
	TSet<int> AddedRoomWalls;
	AddedRoomWalls.Add(Dir);
	AddedRoomWalls.Add(Conjugate(Dir));
	if (Dir % 2 == 0)
	{
		NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls);
	} else
	{
		NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls);
	}
	
	
	AllRooms.Enqueue(NewRoom1);
	AllRooms.Enqueue(NewRoom2);
	AllRooms.Enqueue(NewRoom3);
}

void ALevelGenerator::CreateCorridors(const TPair<int, int> CurrentRoom)
{
	TSet<int> EmptySet;
	for (const int& x : LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors)
	{
		if (LevelMap[CurrentRoom.Key + DirX[x]][CurrentRoom.Value + DirY[x]].Generated == false) {
			const float NewX = (FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + CorridorHeight * RealTileHeight) + CorrDirX[x];
			const float NewY = (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + CorridorHeight * RealTileHeight) + CorrDirY[x];
			FVector NewCorridorLocation = FVector(NewX, NewY, 0.f);
			ARoomActor* NewCorridor = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewCorridorLocation, FRotator(0.f, 90.f, -90.f));
			TSet<int> CorridorDir;
			if (x % 2 == 0)
			{
				CorridorDir.Add(0);
				CorridorDir.Add(2);
				NewCorridor->Init(CorridorDir, CorridorWidth, CorridorHeight, EmptySet);
			} else
			{
				CorridorDir.Add(1);
				CorridorDir.Add(3);
				NewCorridor->Init(CorridorDir, CorridorHeight, CorridorWidth, EmptySet);
			}
			AllRooms.Enqueue(NewCorridor);
		}
	}
}

void ALevelGenerator::Clear()
{
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
			LevelMap[i][j].Generated = false;
			LevelMap[i][j].Main = false;
			LevelMap[i][j].CellRoomType = None;
		}
	}
}

