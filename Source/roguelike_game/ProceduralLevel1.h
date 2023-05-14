// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomActor.h"
#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "ProceduralLevel1.generated.h"


  struct FMapCell {
    TSet<int> Doors;
    bool IsInQueue = false;
    bool WasInQueue = false;
  };


UCLASS()
class ROGUELIKE_GAME_API AProceduralLevel1 : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  AProceduralLevel1();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

private:
  UPROPERTY(EditAnywhere, Category = "Room")
  TSubclassOf<ARoomActor> RoomActorClass;

  uint8 NumOfRooms;

  uint8 LevelNumber = 1;
  static constexpr uint8 MapWidth = 10;
  static constexpr uint8 MapHeight = 10;

  uint8 RoomWidth = 16;
  uint8 RoomHeight = 16;
  uint8 CorridorWidth = 6;
  uint8 CorridorHeight = 7;
  uint8 TileWidth = 32;
  uint8 TileHeight = 32;
  float PixelsPerUnrealUnit = 2.f;
  float RealTileWidth = TileWidth / PixelsPerUnrealUnit;
  float RealTileHeight = TileHeight / PixelsPerUnrealUnit;
  float RealRoomWidth = RoomWidth * RealTileWidth;
  float RealRoomHeight = RoomHeight * RealTileHeight;
  FMapCell LevelMap[MapWidth][MapHeight];

  int8 DirX[4] = {0, 1, 0, -1};
  int8 DirY[4] = {-1, 0, 1, 0};

  float CorrDirX[4] = {RealTileHeight * CorridorHeight,
                      -(RoomHeight / 2 - CorridorWidth / 2) * RealTileHeight,
                      -(RoomHeight *RealTileHeight),
                      -(RoomHeight / 2 - CorridorWidth / 2) * RealTileHeight};
  float CorrDirY[4] = {(RoomWidth / 2 - CorridorWidth / 2) * RealTileWidth,
                      -(RealTileWidth * CorridorHeight),
                      (RoomWidth / 2 - CorridorWidth / 2) * RealTileWidth,
                      RoomWidth * RealTileWidth};

public:
  ARoomActor* CreateRoom(const TPair<int, int> StartRoom, const TPair<int, int> CurrentRoom, TQueue<ARoomActor*> &AllRooms);
  
};
