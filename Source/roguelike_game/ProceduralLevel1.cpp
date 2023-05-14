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

struct MapCell
{
	TSet<int> Doors;
	bool IsInQueue = false;
};

int8 DirX[] = {0, 1, 0, -1};
int8 DirY[] = {-1, 0, 1, 0};

float CorrDirX[] = {112.f, -80.f, -256.f, -80.f};
float CorrDirY[] = {80.f, -112.f, 80.f, 256.f};

void AProceduralLevel1::BeginPlay()
{
	Super::BeginPlay();

	NumOfRooms = FMath::RandRange(0, 1) + 5 + 1 * 2.6;
	uint8 RoomsExist = 0;

	TQueue<TPair<uint8, uint8>> RoomQueue;
	TArray<ARoomActor*> EndRooms;
	TQueue<ARoomActor*> AllRooms;

	MapCell Map[10][10];

	while (true)
	{
		const TPair<uint8, uint8> StartRoom(4, 4);

		RoomQueue.Enqueue(StartRoom);
		RoomsExist++;
		UE_LOG(LogTemp, Warning, TEXT("Add in queue"))

		while (RoomQueue.IsEmpty() != true)
		{
			TPair<uint8, uint8> FromQueue;
			RoomQueue.Peek(FromQueue);
			Map[FromQueue.Key][FromQueue.Value].IsInQueue = false;
			RoomQueue.Pop();
			for (uint8 i = 0; i < 4; i++) {
				if (FromQueue.Key + DirX[i] < 0 || FromQueue.Key + DirX[i] >= 10 || FromQueue.Value + DirY[i] < 0 || FromQueue.Value + DirY[i] >= 10)
				{
					continue;
				}
				if (Map[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].Doors.Num() != 0) {
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
				Map[FromQueue.Key][FromQueue.Value].Doors.Add(i);
				Map[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].Doors.Add((2 + i) % 4);
				if (Map[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].IsInQueue == false)
				{
					RoomQueue.Enqueue(TPair<int, int>(FromQueue.Key + DirX[i],FromQueue.Value + DirY[i]));
					Map[FromQueue.Key + DirX[i]][FromQueue.Value + DirY[i]].IsInQueue = true;
				}
			}
			if (Map[FromQueue.Key][FromQueue.Value].Doors.Num() != 0)
			{
				FVector NewRoomLocation = FVector((StartRoom.Value - FromQueue.Value) * (256.f + 112.f), (StartRoom.Key - FromQueue.Key) * (256.f + 112.f), 0.f);
				ARoomActor* NewRoom = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewRoomLocation, FRotator(0.f, 90.f, -90.f), FActorSpawnParameters());
				NewRoom->Init(Map[FromQueue.Key][FromQueue.Value].Doors, 16, 16);
				AllRooms.Enqueue(NewRoom);
				for (const int& x : Map[FromQueue.Key][FromQueue.Value].Doors)
				{
					FVector NewCorridorLocation = FVector((StartRoom.Value - FromQueue.Value) * (256.f + 112.f) + CorrDirX[x], (StartRoom.Key - FromQueue.Key) * (256.f + 112.f) + CorrDirY[x], 0.f);
					ARoomActor* NewCorridor = GetWorld()->SpawnActor<ARoomActor>(RoomActorClass, NewCorridorLocation, FRotator(0.f, 90.f, -90.f), FActorSpawnParameters());
					TSet<int> CorridorDir;
					if (x % 2 == 0)
					{
						CorridorDir.Add(0);
						CorridorDir.Add(2);
						NewCorridor->Init(CorridorDir, 6, 7);
					} else
					{
						CorridorDir.Add(1);
						CorridorDir.Add(3);
						NewCorridor->Init(CorridorDir, 7, 6);
					}
					AllRooms.Enqueue(NewCorridor);
				}
				if (Map[FromQueue.Key][FromQueue.Value].Doors.Num() == 1)
				{
					EndRooms.Add(NewRoom);
				}
			}
		
		}

		if (RoomsExist == NumOfRooms)
		{
			break;
		} else
		{
			UE_LOG(LogTemp, Warning, TEXT("Bad Room Set"))
			while(AllRooms.IsEmpty() != 0)
			{
				ARoomActor* RoomToDelete;
				AllRooms.Peek(RoomToDelete);
				AllRooms.Pop();
				RoomToDelete->Destroy();
			}
		}
	}
	
}



// Called every frame
void AProceduralLevel1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

