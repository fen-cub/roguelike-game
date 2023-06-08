// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"
#include "EditorDirectories.h"

using RoomPoint = TPair<int, int>;

TArray<TSet<RoomPoint>>
	DefaultTemplatesSet({{RoomPoint(4, 5), RoomPoint(13, 6), RoomPoint(15, 10), RoomPoint(2, 16)},
		{RoomPoint(7, 7), RoomPoint(11, 3), RoomPoint(14, 9), RoomPoint(14, 7)},
		{RoomPoint(11, 16), RoomPoint(13, 9), RoomPoint(3, 10), RoomPoint(7, 3)},
		{RoomPoint(14, 11), RoomPoint(2, 3), RoomPoint(8, 16), RoomPoint(16, 9)},
		{RoomPoint(7, 16), RoomPoint(3, 6), RoomPoint(10, 3), RoomPoint(13, 8)},
		{RoomPoint(3, 8), RoomPoint(10, 4), RoomPoint(15, 8), RoomPoint(11, 13)},
		{RoomPoint(4, 15), RoomPoint(10, 12), RoomPoint(7, 4), RoomPoint(13, 3)},
		{RoomPoint(10, 3), RoomPoint(3, 9), RoomPoint(15, 10), RoomPoint(11, 13)},
		{RoomPoint(3, 11), RoomPoint(6, 3), RoomPoint(12, 8), RoomPoint(12, 16)},
		{RoomPoint(4, 12), RoomPoint(8, 7), RoomPoint(10, 16), RoomPoint(14, 5)},
		{RoomPoint(15, 7), RoomPoint(4, 9), RoomPoint(13, 12), RoomPoint(10, 15)},
		{RoomPoint(9, 5), RoomPoint(5, 8), RoomPoint(15, 10), RoomPoint(14, 15)},
		{RoomPoint(11, 3), RoomPoint(6, 5), RoomPoint(15, 9), RoomPoint(11, 14)},
		{RoomPoint(6, 6), RoomPoint(8, 3), RoomPoint(14, 16), RoomPoint(15, 11)},
		{RoomPoint(12, 10), RoomPoint(3, 9), RoomPoint(8, 16), RoomPoint(12, 3)},
		{RoomPoint(11, 16), RoomPoint(5, 7), RoomPoint(12, 3), RoomPoint(14, 8)},
		{RoomPoint(4, 16), RoomPoint(14, 3), RoomPoint(15, 11), RoomPoint(7, 4)},
		{RoomPoint(15, 3), RoomPoint(4, 12), RoomPoint(14, 12), RoomPoint(11, 16)},
		{RoomPoint(3, 9), RoomPoint(6, 15), RoomPoint(9, 3), RoomPoint(14, 11)},
		{RoomPoint(4, 3), RoomPoint(13, 12), RoomPoint(4, 10), RoomPoint(9, 16)},
		{RoomPoint(11, 17), RoomPoint(16, 10), RoomPoint(16, 6), RoomPoint(6, 4)},
		{RoomPoint(8, 3), RoomPoint(3, 11), RoomPoint(7, 16), RoomPoint(13, 12)},
		{RoomPoint(14, 4), RoomPoint(3, 12), RoomPoint(9, 16), RoomPoint(16, 6)},
		{RoomPoint(3, 16), RoomPoint(5, 7), RoomPoint(12, 13), RoomPoint(14, 3)},
		{RoomPoint(3, 10), RoomPoint(15, 4), RoomPoint(16, 13), RoomPoint(7, 14)},
		{RoomPoint(14, 6), RoomPoint(4, 12), RoomPoint(15, 15), RoomPoint(4, 5)},
		{RoomPoint(14, 15), RoomPoint(8, 3), RoomPoint(4, 11), RoomPoint(15, 5)},
		{RoomPoint(3, 15), RoomPoint(11, 16), RoomPoint(14, 10), RoomPoint(8, 4)},
		{RoomPoint(4, 3), RoomPoint(16, 3), RoomPoint(12, 16), RoomPoint(5, 11)},
		{RoomPoint(7, 16), RoomPoint(6, 8), RoomPoint(11, 3), RoomPoint(14, 10)}});

TArray<TSet<RoomPoint>>
HorizontalAdditionsTemplatesSet({{RoomPoint(16, 1), RoomPoint(11, 3)},
	{RoomPoint(4, 2), RoomPoint(11, 4)},
	{RoomPoint(3, 3), RoomPoint(9, 5)},
	{RoomPoint(2, 4), RoomPoint(10, 1)},
	{RoomPoint(4, 1), RoomPoint(6, 4)},
	{RoomPoint(3, 2), RoomPoint(13, 4)},
	{RoomPoint(5, 3), RoomPoint(15, 2)},
	{RoomPoint(2, 4), RoomPoint(10, 5)},
	{RoomPoint(16, 1), RoomPoint(13, 4)},
	{RoomPoint(14, 1), RoomPoint(5, 3)},
	{RoomPoint(5, 1), RoomPoint(12, 4)},
	{RoomPoint(11, 2), RoomPoint(15, 5)},
	{RoomPoint(4, 1), RoomPoint(13, 3)},
	{RoomPoint(15, 2), RoomPoint(7, 4)},
	{RoomPoint(5, 2), RoomPoint(14, 4)},
	{RoomPoint(5, 4), RoomPoint(12, 2)},
	{RoomPoint(14, 1), RoomPoint(7, 4)},
	{RoomPoint(8, 1), RoomPoint(12, 4)},
	{RoomPoint(5, 1), RoomPoint(11, 4)},
	{RoomPoint(5, 1), RoomPoint(12, 3)}});

TArray<TSet<RoomPoint>>
	VerticalAdditionsTemplatesSet({{RoomPoint(2, 6), RoomPoint(3, 14)},
		{RoomPoint(2, 15), RoomPoint(4, 4)},
		{RoomPoint(5, 4), RoomPoint(2, 17)},
		{RoomPoint(0, 3), RoomPoint(6, 10)},
		{RoomPoint(4, 7), RoomPoint(1, 15)},
		{RoomPoint(1, 3), RoomPoint(4, 8)},
		{RoomPoint(1, 2), RoomPoint(1, 12)},
		{RoomPoint(2, 5), RoomPoint(4, 10)},
		{RoomPoint(5, 17), RoomPoint(1, 5)},
		{RoomPoint(2, 6), RoomPoint(4, 16)},
		{RoomPoint(3, 15), RoomPoint(4, 7)},
		{RoomPoint(1, 13), RoomPoint(3, 4)},
		{RoomPoint(4, 7), RoomPoint(2, 16)},
		{RoomPoint(2, 3), RoomPoint(4, 6)},
		{RoomPoint(1, 16), RoomPoint(3, 7)},
		{RoomPoint(2, 4), RoomPoint(4, 16)},
		{RoomPoint(1, 6), RoomPoint(4, 15)},
		{RoomPoint(4, 4), RoomPoint(2, 10)},
		{RoomPoint(2, 8), RoomPoint(3, 15)},
		{RoomPoint(3, 4), RoomPoint(0, 12)}});

TArray<TSet<RoomPoint>>
	HorizontalCorridorsTemplatesSet({{}});

TArray<TSet<RoomPoint>>
	VerticalCorridorsTemplatesSet({{}});

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

int8 Conjugate(const int8 X)
{
	return (X + 2 + 4) % 4;
}

void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();

	NumOfRooms = FMath::RandRange(0, 1) + 5 + LevelNumber * 2.6;
	MaxBigRooms = LevelNumber - 1;
	MaxLongRooms = LevelNumber;
	MaxLTypeRooms = LevelNumber;
	
	uint8 RoomsExist = 0;
	uint8 BigRoomsExist = 0;
	uint8 LongRoomsExist = 0;
	uint8 LTypeRoomsExist = 0;

	while (true) {
		for (int i = 0; i < DefaultTemplatesSet.Num(); i++)
		{
			UnusedDefaultTemplates.Add(i);
		}
		for (int i = 0; i < HorizontalAdditionsTemplatesSet.Num(); i++)
		{
			UnusedHorizontalAdditionsTemplates.Add(i);
		}
		for (int i = 0; i < VerticalAdditionsTemplatesSet.Num(); i++)
		{
			UnusedVerticalAdditionsTemplates.Add(i);
		}
		for (int i = 0; i < HorizontalCorridorsTemplatesSet.Num(); i++)
		{
			UnusedHorizontalCorridorsTemplates.Add(i);
		}
		for (int i = 0; i < VerticalCorridorsTemplatesSet.Num(); i++)
		{
			UnusedVerticalCorridorsTemplates.Add(i);
		}
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
				if (FromQueue.Key + DirX[i] < 0 || FromQueue.Key + DirX[i] >= MapWidth || FromQueue.Value + DirY[i] < 0 || FromQueue.Value + DirY[i] >= MapHeight)
				{
					continue;
				}
				auto &NeighboringCell = LevelMap[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]];
				if (NeighboringCell.CellRoomType != None) {
					continue;
				}
				if (RoomsExist == NumOfRooms)
				{
					continue;
				}
				if (BigRoomsExist < MaxBigRooms && FMath::RandBool())
				{
					const int Side = CanAddBigRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i);
					if (Side != 0)
					{
						RoomsExist++;
						BigRoomsExist++;
						CurrentCell.Doors.Add(i);
						NeighboringCell.Doors.Add(Conjugate(i));
						SetBigRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i, Side);
						continue;
					}
				}
				if (LongRoomsExist < MaxLongRooms && FMath::RandBool())
				{
					const uint8 LongRoom = CanAddLongRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i);
					if (LongRoom)
					{
						RoomsExist++;
						LongRoomsExist++;
						CurrentCell.Doors.Add(i);
						NeighboringCell.Doors.Add(Conjugate(i));
						SetLongRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), LongRoom);
						continue;
					}
				}
				if (LTypeRoomsExist < MaxLTypeRooms && FMath::RandBool())
				{
					const int8 LTypeRoom = CanAddLTypeRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i);
					if (LTypeRoom)
					{
						RoomsExist++;
						LTypeRoomsExist++;
						CurrentCell.Doors.Add(i);
						NeighboringCell.Doors.Add(Conjugate(i));
						SetLTypeRoom(TPair<int, int>(FromQueue.Key + DirX[i], FromQueue.Value + DirY[i]), i, FMath::Sign(LTypeRoom), FMath::Abs(LTypeRoom));
						continue;
					}
				}
				if (FMath::RandBool())
				{
					RoomsExist++;
					CurrentCell.Doors.Add(i);
					NeighboringCell.Doors.Add(Conjugate(i));
					NeighboringCell.CellRoomType = Default;
					NeighboringCell.Main = true;
					RoomQueue.Enqueue(TPair<int, int>(FromQueue.Key + DirX[i],FromQueue.Value + DirY[i]));
					continue;
				}
			}
			if (CurrentCell.Main)
			{
				if (CurrentCell.CellRoomType == Default)
				{
					CreateDefaultRoom(FromQueue);
					CurrentCell.Generated = true;
				}
				if (CurrentCell.CellRoomType == Big)
				{
					CreateBigRoom(FromQueue, LevelMap[FromQueue.Key][FromQueue.Value].Direction, LevelMap[FromQueue.Key][FromQueue.Value].Side);
				}
				if (CurrentCell.CellRoomType == Long)
				{
					CreateLongRoom(FromQueue, LevelMap[FromQueue.Key][FromQueue.Value].Direction);
				}
				if (CurrentCell.CellRoomType == LType)
				{
					CreateLTypeRoom(FromQueue, LevelMap[FromQueue.Key][FromQueue.Value].Direction, LevelMap[FromQueue.Key][FromQueue.Value].Side);
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
	const int Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
	const int Num = UnusedDefaultTemplates[Index];
	UnusedDefaultTemplates.Remove(Index);
	UE_LOG(LogTemp, Warning, TEXT("Num %d"), Num)
	NewRoom->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, EmptySet, 0, Num);
	AllRooms.Enqueue(NewRoom);
	CreateCorridors(CurrentRoom);
}


int ALevelGenerator::CanAddLongRoom(const TPair<uint8, uint8> StartRoom, uint8 Dir) const
{
	if (FMath::RandBool())
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
	}
	return 0;
}

void ALevelGenerator::SetLongRoom(const TPair<int, int> CurrentRoom, int Dir)
{
	Dir -= 2;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].CellRoomType = Long;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Main = true;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Direction = Dir;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls.Add(Dir);
	LevelMap[CurrentRoom.Key + DirX[Dir]][CurrentRoom.Value + DirY[Dir]].CellRoomType = Long;
	LevelMap[CurrentRoom.Key + DirX[Dir]][CurrentRoom.Value + DirY[Dir]].Walls.Add(Conjugate(Dir));
	RoomQueue.Enqueue(TPair<int, int>(CurrentRoom.Key + DirX[Dir], CurrentRoom.Value + DirY[Dir]));
	RoomQueue.Enqueue(CurrentRoom);
}

void ALevelGenerator::CreateLongRoom(const TPair<int, int> CurrentRoom, int Dir)
{
	TSet<int> EmptySet;
	const TPair<int, int> SecondRoom = TPair<int, int>(CurrentRoom.Key + DirX[Dir], CurrentRoom.Value + DirY[Dir]);
	const FVector CurRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom1 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, CurRoomLocation, FRotator(0.f, 90.f, -90.f));
	int Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
	int Num = UnusedDefaultTemplates[Index];
	UnusedDefaultTemplates.Remove(Index);
	NewRoom1->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
	CreateCorridors(CurrentRoom);
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
	
	
	const FVector SecondRoomLocation = FVector((FirstRoom.Value - SecondRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - SecondRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	ARoomActor* NewRoom2 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, SecondRoomLocation, FRotator(0.f, 90.f, -90.f));
	Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
	Num = UnusedDefaultTemplates[Index];
	UnusedDefaultTemplates.Remove(Index);
	NewRoom2->Init(LevelMap[SecondRoom.Key][SecondRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[SecondRoom.Key][SecondRoom.Value].Walls, 0, Num);
	CreateCorridors(SecondRoom);
	LevelMap[SecondRoom.Key][SecondRoom.Value].Generated = true;

	
	const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight) + AddDirX[Dir], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight) + AddDirY[Dir], 0.f);
	ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
	TSet<int> AddedRoomWalls;
	AddedRoomWalls.Add(Dir);
	AddedRoomWalls.Add(Conjugate(Dir));
	if (Dir % 2 == 0)
	{
		Index = FMath::RandRange(0, UnusedHorizontalAdditionsTemplates.Num() - 1);
		Num = UnusedHorizontalAdditionsTemplates[Index];
		UnusedHorizontalAdditionsTemplates.Remove(Index);
		NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls, 0, Num);
	} else
	{
		Index = FMath::RandRange(0, UnusedVerticalAdditionsTemplates.Num() - 1);
		Num = UnusedVerticalAdditionsTemplates[Index];
		UnusedVerticalAdditionsTemplates.Remove(Index);
		NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls, 0, Num);
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
				NewCorridor->Init(CorridorDir, CorridorWidth, CorridorHeight, EmptySet, 0, 0);
			} else
			{
				CorridorDir.Add(1);
				CorridorDir.Add(3);
				NewCorridor->Init(CorridorDir, CorridorHeight, CorridorWidth, EmptySet, 0, 0);
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
	UnusedDefaultTemplates.Empty();
	UnusedHorizontalAdditionsTemplates.Empty();
	UnusedVerticalAdditionsTemplates.Empty();
	UnusedHorizontalCorridorsTemplates.Empty();
	UnusedVerticalCorridorsTemplates.Empty();
}

int ALevelGenerator::CanAddBigRoom(const TPair<int, int> CurrentRoom, int Direction) const {
	TPair<int, int> CurrentRoomCopy1 = CurrentRoom;
	if (FMath::RandBool())
	{
		for (uint8 i = 0; i < 3; i++) {
			CurrentRoomCopy1.Key += DirX[(Direction + i) % 4];
			CurrentRoomCopy1.Value += DirY[(Direction + i) % 4];
			if (CurrentRoomCopy1.Key < 0 || CurrentRoomCopy1.Key >= MapWidth || CurrentRoomCopy1.Value < 0 || CurrentRoomCopy1.Value >= MapHeight) {
				break;
			}
			if (LevelMap[CurrentRoomCopy1.Key][CurrentRoomCopy1.Value].CellRoomType != None)
			{
				break;
			}
			if (i == 2 && FMath::RandBool()) return 1;
		}
		TPair<int, int> CurrentRoomCopy2 = CurrentRoom;
		for (uint8 i = 0; i < 3; i++) {
			CurrentRoomCopy2.Key += DirX[(Direction - i) % 4];
			CurrentRoomCopy2.Value += DirY[(Direction - i) % 4];
			if (CurrentRoomCopy2.Key < 0 || CurrentRoomCopy2.Key >= MapWidth || CurrentRoomCopy2.Value < 0 || CurrentRoomCopy2.Value >= MapHeight)
			{
				break;
			}
			if (LevelMap[CurrentRoomCopy2.Key][CurrentRoomCopy2.Value].CellRoomType != None)
			{
				break;
			}
			if (i == 2 && FMath::RandBool()) return -1;
		}
	}
	
	return 0;
}


void ALevelGenerator::SetBigRoom(const TPair<int, int> CurrentRoom, int Dir, int Side)
{
	TPair<int, int> CurrentRoomCopy = CurrentRoom;
	for (int i = 0; i <= 3; i++) {
		LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add((Dir + Side * i + 4) % 4);
		LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add((Dir + Side * (i + 1) + 4) % 4);
		CurrentRoomCopy.Key += DirX[(Dir + Side * i + 4) % 4];
		CurrentRoomCopy.Value += DirY[(Dir + Side * i + 4) % 4];
		
		LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].CellRoomType = Big;
		RoomQueue.Enqueue(CurrentRoomCopy);
		if (i == 3)
		{
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Main = true;
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Direction = Dir;
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Side = Side;
		}
	}
}

void ALevelGenerator::CreateBigRoom(TPair<int, int> CurrentRoom, int Dir, int Side)
{
	TSet<int> EmptySet;
	for (int i = 0; i <= 3; i++)
	{
		const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * (CorridorHeight)) + AddDirX[(Dir + Side * i + 4) % 4], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * (CorridorHeight)) + AddDirY[(Dir + Side * i + 4) % 4], 0.f);
		ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
		TSet<int> AddedRoomWalls;
		AddedRoomWalls.Add((Dir + Side * i + 4) % 4);
		AddedRoomWalls.Add(Conjugate((Dir + Side * i + 4) % 4));
		if (((Dir + Side * i + 4) % 4) % 2 == 0)
		{
			const int Index = FMath::RandRange(0, UnusedHorizontalAdditionsTemplates.Num() - 1);
			const int Num = UnusedHorizontalAdditionsTemplates[Index];
			UnusedHorizontalAdditionsTemplates.Remove(Index);
			NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls, 0, Num);
		} else
		{
			const int Index = FMath::RandRange(0, UnusedVerticalAdditionsTemplates.Num() - 1);
			const int Num = UnusedVerticalAdditionsTemplates[Index];
			UnusedVerticalAdditionsTemplates.Remove(Index);
			NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls, 0, Num);
		}
		AllRooms.Enqueue(NewRoom3);
		CurrentRoom.Key += DirX[(Dir + Side * i + 4) % 4];
		CurrentRoom.Value += DirY[(Dir + Side * i + 4) % 4];
		const FVector CurRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
		ARoomActor* NewRoom1 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, CurRoomLocation, FRotator(0.f, 90.f, -90.f));
		const int Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
		const int Num = UnusedDefaultTemplates[Index];
		UnusedDefaultTemplates.Remove(Index);
		NewRoom1->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
		CreateCorridors(CurrentRoom);
		LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
		AllRooms.Enqueue(NewRoom1);
	}
	
}

int ALevelGenerator::CanAddLTypeRoom(const TPair<uint8, uint8> CurrentRoom, uint8 Dir) const
{
	TPair<int, int> CurrentRoomCopy1;
	constexpr int Side[2] = {1, -1};
	if (FMath::RandBool())
	{
		for (int j = 0; j < 2; j++)
		{
			CurrentRoomCopy1 = CurrentRoom;
			for (int i = 0; i < 2; i++)
			{
				CurrentRoomCopy1.Key += DirX[(Dir + i * Side[j] + 4) % 4];
				CurrentRoomCopy1.Value += DirY[(Dir + i * Side[j] + 4) % 4];
				if (CurrentRoomCopy1.Key < 0 || CurrentRoomCopy1.Key >= MapWidth || CurrentRoomCopy1.Value < 0 || CurrentRoomCopy1.Value >= MapHeight)
				{
					break;
				}
				if (LevelMap[CurrentRoomCopy1.Key][CurrentRoomCopy1.Value].CellRoomType != None)
				{
					break;
				}
				if (i == 1 && FMath::RandBool())
				{
					return 1 * Side[j];
				}
			}
		}
	}
	if (FMath::RandBool())
	{
		for (int j = 0; j < 2; j++)
		{
			CurrentRoomCopy1 = CurrentRoom;
			for (int i = 1; i >= 0; i--)
			{
				CurrentRoomCopy1.Key += DirX[(Dir + i * Side[j] + 4) % 4];
				CurrentRoomCopy1.Value += DirY[(Dir + i * Side[j] + 4) % 4];
				if (CurrentRoomCopy1.Key < 0 || CurrentRoomCopy1.Key >= MapWidth || CurrentRoomCopy1.Value < 0 || CurrentRoomCopy1.Value >= MapHeight)
				{
					break;
				}
				if (LevelMap[CurrentRoomCopy1.Key][CurrentRoomCopy1.Value].CellRoomType != None)
				{
					break;
				}
				if (i == 0 && FMath::RandBool())
				{
					return 3 * Side[j];
				}
			}
		}
	}
	if (FMath::RandBool())
	{
		for (int j = 0; j < 2; j++)
		{
			CurrentRoomCopy1 = CurrentRoom;
			for (int i = 0; i < 2; i++)
			{
				CurrentRoomCopy1.Key += DirX[(Dir + i * Side[j] + 4) % 4];
				CurrentRoomCopy1.Value += DirY[(Dir + i  * Side[j] + 4) % 4];
				if (CurrentRoomCopy1.Key < 0 || CurrentRoomCopy1.Key >= MapWidth || CurrentRoomCopy1.Value < 0 || CurrentRoomCopy1.Value >= MapHeight)
				{
					break;
				}
				if (LevelMap[CurrentRoomCopy1.Key][CurrentRoomCopy1.Value].CellRoomType != None)
				{
					break;
				}
				CurrentRoomCopy1.Key += DirX[Conjugate(Dir + i * Side[j])];
				CurrentRoomCopy1.Value += DirY[Conjugate(Dir + i * Side[j])];
				if (i == 1 && FMath::RandBool())
				{
					return 2 * Side[j];
				}
			}
		}
	}
	return 0;
}

void ALevelGenerator::SetLTypeRoom(const TPair<int, int> CurrentRoom, int Dir, int Side, int Type)
{
	TPair<int, int> CurrentRoomCopy = CurrentRoom;
	switch (Type){
	case 1:
		for (int i = 0; i < 2; i++)
		{
			CurrentRoomCopy.Key += DirX[(Dir + Side * i + 4) % 4];
			CurrentRoomCopy.Value += DirY[(Dir + Side * i + 4) % 4];
			if (i == 0)
			{
				LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add((Dir + Side * (i + 1) + 4) % 4);
			}
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add(Conjugate(Dir + Side * i));
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].CellRoomType = LType;
			RoomQueue.Enqueue(CurrentRoomCopy);
		}
		break;
	case 2:
		for (int i = 0; i < 2; i++)
		{
			CurrentRoomCopy.Key += DirX[(Dir + i * Side + 4) % 4];
			CurrentRoomCopy.Value += DirY[(Dir + i  * Side + 4) % 4];
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].CellRoomType = LType;
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add(Conjugate(Dir + i * Side));
			RoomQueue.Enqueue(CurrentRoomCopy);
			CurrentRoomCopy.Key += DirX[Conjugate(Dir + i * Side)];
			CurrentRoomCopy.Value += DirY[Conjugate(Dir + i  * Side)];
		}
		break;
	default:
		for (int i = 1; i >= 0; i--)
		{
			CurrentRoomCopy.Key += DirX[(Dir + Side * i + 4) % 4];
			CurrentRoomCopy.Value += DirY[(Dir + Side * i + 4) % 4];
			if (i == 1)
			{
				LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add((Dir + Side * (i - 1) + 4) % 4);
			}
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].Walls.Add(Conjugate(Dir + Side * i));
			LevelMap[CurrentRoomCopy.Key][CurrentRoomCopy.Value].CellRoomType = LType;
			RoomQueue.Enqueue(CurrentRoomCopy);
		}
		break;
	}
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].CellRoomType = LType;
	if (Type != 3)
	{
		LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls.Add(Dir);
	}
	if (Type != 1)
	{
		LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls.Add((Dir + Side + 4) % 4);
	}
	RoomQueue.Enqueue(CurrentRoom);
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Main = true;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Direction = Dir;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Side = Side;
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].RoomType = Type;
}

void ALevelGenerator::CreateLTypeRoom(TPair<int, int> CurrentRoom, int Dir, int Side)
{
	TSet<int> EmptySet;
	const int Type = LevelMap[CurrentRoom.Key][CurrentRoom.Value].RoomType;
	const FVector CurRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
	int Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
	int Num = UnusedDefaultTemplates[Index];
	UnusedDefaultTemplates.Remove(Index);
	ARoomActor* NewRoom1 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, CurRoomLocation, FRotator(0.f, 90.f, -90.f));
	NewRoom1->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
	CreateCorridors(CurrentRoom);
	LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;

	switch (Type)
	{
	case 1:
		for (int i = 0; i < 2; i++)
		{
			const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * (CorridorHeight)) + AddDirX[(Dir + Side * i + 4) % 4], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * (CorridorHeight)) + AddDirY[(Dir + Side * i + 4) % 4], 0.f);
			ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
			TSet<int> AddedRoomWalls;
			AddedRoomWalls.Add((Dir + Side * i + 4) % 4);
			AddedRoomWalls.Add(Conjugate((Dir + Side * i + 4) % 4));
			if (((Dir + Side * i + 4) % 4) % 2 == 0)
			{
				Index = FMath::RandRange(0, UnusedHorizontalAdditionsTemplates.Num() - 1);
				Num = UnusedHorizontalAdditionsTemplates[Index];
				UnusedHorizontalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls, 0, Num);
			} else
			{
				Index = FMath::RandRange(0, UnusedVerticalAdditionsTemplates.Num() - 1);
				Num = UnusedVerticalAdditionsTemplates[Index];
				UnusedVerticalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls, 0, Num);
			}
			AllRooms.Enqueue(NewRoom3);
			CurrentRoom.Key += DirX[(Dir + Side * i + 4) % 4];
			CurrentRoom.Value += DirY[(Dir + Side * i + 4) % 4];
			const FVector NewRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
			ARoomActor* NewRoom2 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f));
			Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
			Num = UnusedDefaultTemplates[Index];
			UnusedDefaultTemplates.Remove(Index);
			NewRoom2->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
			CreateCorridors(CurrentRoom);
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
			AllRooms.Enqueue(NewRoom1);
		}
		break;
	case 2:
		for (int i = 0; i < 2; i++)
		{
			const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * (CorridorHeight)) + AddDirX[(Dir + Side * i + 4) % 4], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * (CorridorHeight)) + AddDirY[(Dir + Side * i + 4) % 4], 0.f);
			ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
			TSet<int> AddedRoomWalls;
			AddedRoomWalls.Add((Dir + Side * i + 4) % 4);
			AddedRoomWalls.Add(Conjugate((Dir + Side * i + 4) % 4));
			if (((Dir + Side * i + 4) % 4) % 2 == 0)
			{
				Index = FMath::RandRange(0, UnusedHorizontalAdditionsTemplates.Num() - 1);
				Num = UnusedHorizontalAdditionsTemplates[Index];
				UnusedHorizontalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls, 0, Num);
			} else
			{
				Index = FMath::RandRange(0, UnusedVerticalAdditionsTemplates.Num() - 1);
				Num = UnusedVerticalAdditionsTemplates[Index];
				UnusedVerticalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls, 0, Num);
			}
			AllRooms.Enqueue(NewRoom3);
			CurrentRoom.Key += DirX[(Dir + Side * i + 4) % 4];
			CurrentRoom.Value += DirY[(Dir + Side * i + 4) % 4];
			const FVector NewRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
			ARoomActor* NewRoom2 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f));
			Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
			Num = UnusedDefaultTemplates[Index];
			UnusedDefaultTemplates.Remove(Index);
			NewRoom2->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
			CreateCorridors(CurrentRoom);
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
			AllRooms.Enqueue(NewRoom1);
			CurrentRoom.Key -= DirX[(Dir + Side * i + 4) % 4];
			CurrentRoom.Value -= DirY[(Dir + Side * i + 4) % 4];
		}
		break;
	default:
		for (int i = 1; i >= 0; i--)
		{
			const FVector AddedRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * (CorridorHeight)) + AddDirX[(Dir + Side * i + 4) % 4], (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * (CorridorHeight)) + AddDirY[(Dir + Side * i + 4) % 4], 0.f);
			ARoomActor* NewRoom3 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, AddedRoomLocation, FRotator(0.f, 90.f, -90.f));
			TSet<int> AddedRoomWalls;
			AddedRoomWalls.Add((Dir + Side * i + 4) % 4);
			AddedRoomWalls.Add(Conjugate((Dir + Side * i + 4) % 4));
			if (((Dir + Side * i + 4) % 4) % 2 == 0)
			{
				Index = FMath::RandRange(0, UnusedHorizontalAdditionsTemplates.Num() - 1);
				Num = UnusedHorizontalAdditionsTemplates[Index];
				UnusedHorizontalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, RoomWidth, CorridorHeight, AddedRoomWalls, 0, Num);
			} else
			{
				Index = FMath::RandRange(0, UnusedVerticalAdditionsTemplates.Num() - 1);
				Num = UnusedVerticalAdditionsTemplates[Index];
				UnusedVerticalAdditionsTemplates.Remove(Index);
				NewRoom3->Init(EmptySet, CorridorHeight, RoomHeight, AddedRoomWalls, 0, Num);
			}
			AllRooms.Enqueue(NewRoom3);
			CurrentRoom.Key += DirX[(Dir + Side * i + 4) % 4];
			CurrentRoom.Value += DirY[(Dir + Side * i + 4) % 4];
			const FVector NewRoomLocation = FVector((FirstRoom.Value - CurrentRoom.Value) * (RealRoomHeight + RealTileHeight * CorridorHeight), (FirstRoom.Key - CurrentRoom.Key) * (RealRoomWidth + RealTileHeight * CorridorHeight), 0.f);
			ARoomActor* NewRoom2 = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f));
			Index = FMath::RandRange(0, UnusedDefaultTemplates.Num() - 1);
			Num = UnusedDefaultTemplates[Index];
			UnusedDefaultTemplates.Remove(Index);
			NewRoom2->Init(LevelMap[CurrentRoom.Key][CurrentRoom.Value].Doors, RoomWidth, RoomHeight, LevelMap[CurrentRoom.Key][CurrentRoom.Value].Walls, 0, Num);
			CreateCorridors(CurrentRoom);
			LevelMap[CurrentRoom.Key][CurrentRoom.Value].Generated = true;
			AllRooms.Enqueue(NewRoom1);
		}
		break;
	}

	AllRooms.Enqueue(NewRoom1);
}


