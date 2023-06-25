// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomActor.h"
#include "Items/AttributesRecoveryItem.h"
#include "Items/ArmorItem.h"
#include "Items/ArtifactItem.h"
#include "Items/WeaponItem.h"
#include "Items/ArtifactItem.h"
#include "InteractiveActors/Storage.h"
#include "InteractiveActors/LevelTeleport.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h" 
#include "LevelGenerator.generated.h"

enum ERoomType {
  Default,
  Long,
  LType,
  Big,
  Empty
};


  struct FMapCell {
    TArray<int> Doors;
    TArray<int> Walls;
    ERoomType CellRoomType = Empty;
    bool Main = false;
    bool Generated = false;
    int Direction = 0;
    int Side = 0;
    int RoomType = 0;
  };


UCLASS()
class ROGUELIKE_GAME_API ALevelGenerator : public AActor {
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ALevelGenerator();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
  
  UPROPERTY(EditAnywhere, Replicated, Category = "Room")
  TSubclassOf<ARoomActor> RoomActorClass = ARoomActor::StaticClass();

  uint8 NumOfRooms;

  uint8 LevelNumber;
  static constexpr uint8 MapWidth = 10;
  static constexpr uint8 MapHeight = 10;

  uint8 MaxBigRooms;
  uint8 MaxLongRooms;
  uint8 MaxLTypeRooms;

  TPair<int, int> LastRoom;

  uint8 RoomWidth = 20;
  uint8 RoomHeight = 20;
  uint8 CorridorWidth = 6;
  uint8 CorridorHeight = 7;
  uint8 TileWidth = 16;
  uint8 TileHeight = 16;
  float PixelsPerUnrealUnit = 1.f;
  float RealTileWidth = TileWidth / PixelsPerUnrealUnit;
  float RealTileHeight = TileHeight / PixelsPerUnrealUnit;
  float RealRoomWidth = RoomWidth * RealTileWidth;
  float RealRoomHeight = RoomHeight * RealTileHeight;
  FMapCell LevelMap[MapWidth][MapHeight];

  const TPair<uint8, uint8> FirstRoom = TPair<uint8, uint8>(MapWidth / 2, MapHeight / 2);

  TQueue<TPair<uint8, uint8>> RoomQueue;

  TArray<ARoomActor*> AllRooms;

  int8 DirX[4] = {0, 1, 0, -1};
  int8 DirY[4] = {-1, 0, 1, 0};

  TArray<int> UnusedDefaultTemplates;
  TArray<int> UnusedHorizontalAdditionsTemplates;
  TArray<int> UnusedVerticalAdditionsTemplates;
  TArray<int> UnusedHorizontalCorridorsTemplates;
  TArray<int> UnusedVerticalCorridorsTemplates;

  float CorrDirX[4] = {RealTileHeight * CorridorHeight,
                      -(RoomHeight / 2 - CorridorWidth / 2) * RealTileHeight,
                      -(RoomHeight *RealTileHeight),
                      -(RoomHeight / 2 - CorridorWidth / 2) * RealTileHeight};
  float CorrDirY[4] = {(RoomWidth / 2 - CorridorWidth / 2) * RealTileWidth,
                      -(RealTileWidth * CorridorHeight),
                      (RoomWidth / 2 - CorridorWidth / 2) * RealTileWidth,
                      RoomWidth * RealTileWidth};

  float AddDirX[4] = {RealTileHeight * CorridorHeight,
  0,
  -(RoomHeight *RealTileHeight),
    0};
  float AddDirY[4] = {0,
    -(RealTileWidth * CorridorHeight),
    0,
    RoomWidth * RealTileWidth
  };

  float SquareX[4] = {
    - (RoomHeight - 1) * RealTileHeight,
    - (RoomHeight - 1) * RealTileHeight,
    0,
    0
  };
  float SquareY[4] = {
    0,
    (RoomWidth - 1) * RealTileWidth,
    (RoomWidth - 1) * RealTileWidth,
    0
  };

  void CreateDefaultRoom(const TPair<uint8, uint8> CurrentRoom);

  void CreateBigRoom(TPair<int, int> CurrentRoom, int Dir, int Side);

  int CanAddBigRoom(const TPair<int, int> CurrentRoom, int Direction) const;

  int CanAddLongRoom(const TPair<uint8, uint8> CurrentRoom, uint8 Dir) const;

  void SetLongRoom(const TPair<int, int> CurrentRoom, int Dir);

  void CreateLongRoom(const TPair<int, int> CurrentRoom, int Dir);

  void CreateCorridors(const TPair<int, int> CurrentRoom);

  void Clear();

  UClass* PickRandItem();

  void SetBigRoom(const TPair<int, int> CurrentRoom, int Dir, int Side);

  int CanAddLTypeRoom(const TPair<uint8, uint8> CurrentRoom, uint8 Dir) const;

  void SetLTypeRoom(const TPair<int, int> CurrentRoom, int Dir, int Side, int Type);

  void CreateLTypeRoom(TPair<int, int> CurrentRoom, int Dir, int Side);

virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

  void SpawnItem(UClass* ItemToSpawn, FVector Location);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AAttributesRecoveryItem> StaminaRecoveryItemClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AAttributesRecoveryItem> HealthRecoveryItemClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AAttributesRecoveryItem> HealthDecreaseItemClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AArtifactItem> BootsClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AArmorItem> ArmorItemClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AWeaponItem> SwordClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<AStorage> StorageClass;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
  TSubclassOf<ALevelTeleport> LevelTeleportClass;
  
};
