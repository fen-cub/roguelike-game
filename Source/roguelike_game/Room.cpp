// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"

// Rotation -90 180 180

void URoom::CreateRoom(const uint8 Width, const uint8 Height, const uint8 Side)
{
	RoomWidth = Width;
	RoomHeight = Height;
	CreateNewTileMap(Width, Height, 16, 16, 1.0f);
	AddNewLayer();

	/*
	UPaperTileSet *TileSet = LoadObject<UPaperTileSet>(
	nullptr, TEXT("/Game/Textures/TX_Tileset_Grass_TileSet"));
	*/
	
	
	UPaperTileSet *TileSet = LoadObject<UPaperTileSet>(
	nullptr, TEXT("/Game/Textures/Tiles_TileSet"));
	
	

	UPaperTileMap *BaseMap = nullptr;

	if (Width == 1 && Height == 1)
	{
		switch (Side)
		{
		case 0:
			BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/Square0"));
			break;
		case 1:
			BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/Square1"));
			break;
		case 2:
			BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/Square2"));
			break;
		default:
			BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/Square3"));
			break;
		}
	} else if (Width == 6 && Height == 7)
	{
		BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/MapCprridor"));
	} else if (Width == 7 && Height == 6)
	{
		BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/MapCorridorGorizontal"));
	} else if (Width == 20 && Height == 7)
	{
		BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/AddedRoomGorizontal"));
	} else if (Width == 7 && Height == 20)
	{
		BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/AddedRoomVertical"));
	} else
	{
		BaseMap = LoadObject<UPaperTileMap>(nullptr, TEXT("/Game/Textures/MyMap"));
	}

	if (BaseMap)
	{
		UE_LOG(LogTemp, Warning, TEXT("BaseMap ok"))
		//UPaperTileLayer* NewLayer = BaseMap->AddNewLayer();
	}
	SetTileMap(BaseMap);
	/*
	for (auto CurLayer : BaseMap->TileLayers)
	{
		const UPaperTileLayer* NewLayer = this->AddNewLayer();
		//NewLayer = CurLayer;
	}
	*/

	MakeTileMapEditable();
	AddNewLayer();
	AddNewLayer();
	

	//NewLayer = URoomLayer1();
	
	//BaseMap->CloneTileMap(this);
	FPaperTileInfo TileInfo;
	TileInfo.TileSet = TileSet;
	TileInfo.PackedTileIndex = 76;
	//NewLayer->SetCell(0, 0, TileInfo);
	
	if (Width == 20 && Height == 20)
	{
		for (int i = 0; i < 4; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("In for loop"))
			//FPaperTileInfo TileInfo;
			TileInfo.TileSet = TileSet;
			if (Walls[i])
			{
				if (i % 2 == 0)
				{
					for (int j = 0; j < RoomWidth; j++)
					{
						if (!(Doors[i] && (j == RoomWidth / 2 - 2 || j == RoomWidth / 2 - 1 || j == RoomWidth / 2  || j == RoomWidth / 2 + 1)))
						{
							UE_LOG(LogTemp, Warning, TEXT("X = %d, Y = %d"), j, (i == 0 ? 0 : RoomHeight - 1))
							TileInfo.PackedTileIndex = (i == 0 ? 101 : 76);
							//NewLayer->SetCell(j, (i == 0 ? 0 : RoomHeight - 1), TileInfo);
							SetTile(j,  (i == 0 ? 0 : RoomHeight - 1) , 0, TileInfo);
							TileInfo.PackedTileIndex = (i == 0 ? 5 : 55);
							//NewLayer->SetCell(j, (i == 0 ? 0 : RoomHeight - 1), TileInfo);
							SetTile(j,  (i == 0 ? 1 : RoomHeight - 2) , 1, TileInfo);
						}
					}
				} else
				{
					for (int j = 0; j < RoomHeight; j++)
					{
						if (!(Doors[i] && (j == RoomHeight / 2 - 2 || j == RoomHeight / 2 - 1 || j == RoomHeight / 2  || j == RoomHeight / 2 + 1)))
						{
							UE_LOG(LogTemp, Warning, TEXT("X = %d, Y = %d"), (i == 1 ? RoomWidth - 1 : 0),  j)
							TileInfo.PackedTileIndex = (i == 1 ? 128 : 129);
							//NewLayer->SetCell((i == 1 ? RoomHeight - 1: 0), j, TileInfo);
							SetTile((i == 1 ? 0 : RoomHeight - 1),  j , 0, TileInfo);
							TileInfo.PackedTileIndex = (i == 1 ? 29 : 31);
							//NewLayer->SetCell((i == 1 ? RoomHeight - 1: 0), j, TileInfo);
							SetTile((i == 1 ? 1 : RoomHeight - 2),  j , 1, TileInfo);
						}
					}
				}
				if (Doors[i])
				{
					TileInfo.PackedTileIndex = (i == 2 ? 77 : (i == 3) ? 75 : 102);
					int X = (i == 0 || i == 2) ? RoomWidth / 2 - 3 : (i == 1 ? 0 :  RoomWidth - 1);
					int Y = (i == 0) ? 0 : (i == 1 ? RoomHeight / 2 - 3 : (i == 2) ? RoomHeight - 1: RoomHeight / 2 + 2);
					SetTile(X,  Y , 0, TileInfo);
					TileInfo.PackedTileIndex = (i % 2 == 0 ? 29 : 5);
					SetTile(i % 2 == 0 ? RoomWidth / 2 - 2 : X,  i % 2 == 1 ? RoomHeight / 2 - 2 : Y , 1, TileInfo);
					TileInfo.PackedTileIndex = ((i == 0 || i == 1) ? 59 : i == 2 ? 9 : 57);
					SetTile(i % 2 == 0 ? RoomWidth / 2 - 2 : X + (2 - i),  i % 2 == 1 ? RoomHeight / 2 - 2 : (Y + (1 - i)), 1, TileInfo);
					TileInfo.PackedTileIndex = (i == 2 ? 75 : (i == 1) ? 77 : 100);
					X = (i == 0 || i == 2) ? RoomWidth / 2 + 2 : (i == 1 ? 0 :  RoomWidth - 1);
					Y = (i == 0) ? 0 : (i == 1 ? RoomHeight / 2 + 2 : (i == 2) ? RoomHeight - 1: RoomHeight / 2 - 3);
					SetTile(X,  Y , 0, TileInfo);
					TileInfo.PackedTileIndex = (i % 2 == 0 ? 31 : 55);
					SetTile(i % 2 == 0 ? RoomWidth / 2 + 1 : X,  i % 2 == 1 ? RoomHeight / 2 + 1 : Y , 1, TileInfo);
					TileInfo.PackedTileIndex = ((i == 2 || i == 3) ? 7 : (i == 0) ? 57 : 9);
					SetTile(i % 2 == 0 ? RoomWidth / 2 + 1 : X + (2 - i),  i % 2 == 1 ? RoomHeight / 2 + 1 : Y + (1 - i), 1, TileInfo);
				}
			}
		}
		if ((Walls[0] || Doors[0]) && (Walls[1] || Doors[1]) || (!Walls[0] && !Walls[1]))
		{
			TileInfo.PackedTileIndex = (!Walls[0] && !Walls[1]) ? 102 : 103;
			SetTile(0,0, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[0] && !Walls[1]) ? 59 : 4;
			SetTile(1,1, 1, TileInfo);
			if ((Walls[0] || Doors[0]) && (Walls[1] || Doors[1]))
			{
				TileInfo.PackedTileIndex = 29;
				SetTile(0,1, 1, TileInfo);
				TileInfo.PackedTileIndex = 5;
				SetTile(1,0, 1, TileInfo);
			}
		}
		if ((Walls[1] || Doors[1]) && (Walls[2] || Doors[2]) || (!Walls[1] && !Walls[2]))
		{
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 77 : 153;
			SetTile(0 ,RoomHeight - 1, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 9 : 54;
			SetTile(1 ,RoomHeight - 2, 1, TileInfo);
			if ((Walls[1] || Doors[1]) && (Walls[2] || Doors[2]))
			{
				TileInfo.PackedTileIndex = 29;
				SetTile(0,RoomHeight - 2, 1, TileInfo);
				TileInfo.PackedTileIndex = 55;
				SetTile(1,RoomHeight - 1, 1, TileInfo);
			}
		}
		if ((Walls[2] || Doors[2]) && (Walls[3] || Doors[3]) || (!Walls[2] && !Walls[3]))
		{
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 75 : 154;
			SetTile(RoomHeight - 1,RoomWidth - 1, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 7 : 56;
			SetTile(RoomHeight - 2,RoomWidth - 2, 1, TileInfo);
			if ((Walls[2] || Doors[2]) && (Walls[3] || Doors[3]))
			{
				TileInfo.PackedTileIndex = 55;
				SetTile(RoomHeight - 2,RoomHeight - 1, 1, TileInfo);
				TileInfo.PackedTileIndex = 31;
				SetTile(RoomHeight - 1, RoomHeight - 2, 1, TileInfo);
			}
		}
		if ((Walls[3] || Doors[3]) && (Walls[0] || Doors[0]) || (!Walls[3] && !Walls[0]))
		{
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 100 : 104;
			SetTile(RoomWidth - 1, 0, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 57 : 6;
			SetTile(RoomWidth - 2, 1, 1, TileInfo);
			if ((Walls[3] || Doors[3]) && (Walls[0] || Doors[0]))
			{
				TileInfo.PackedTileIndex = 31;
				SetTile(RoomHeight - 1,1, 1, TileInfo);
				TileInfo.PackedTileIndex = 5;
				SetTile(RoomHeight - 2,0, 1, TileInfo);
			}
		}
	}
	

	/*
	if ((Walls [i] || Doors[i]) && (Walls[(i - 1 + 4) % 4] || Doors[(i - 1 + 4) % 4]))
	{
		TileInfo.PackedTileIndex = (i == 0 ? 103 : (i == 1 ? 153 : (i == 2 ? 154 : 104)));
		SetTile(((i == 0  || i == 2) ? 0 : RoomHeight - 1),  (i == 0  || i == 3) ? 0 : RoomWidth - 1 , 0, TileInfo);
	}
	*/
	
	SetLayerCollision(0, true, true, 100);

	/*
	
	FPaperTileInfo TileInfo;
	TileInfo.TileSet = TileSet;

	for (int32 X = 0; X < Width; ++X) {
		for (int32 Y = 0; Y < Height; ++Y) {
			if (Y == 0 && Walls[0])
			{
				if (!(Doors[0] && (X == RoomWidth / 2 - 2 || X == RoomWidth / 2 - 1 || X == RoomWidth / 2  || X == RoomWidth / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (X == 0 && Walls[1])
			{
				if (!(Doors[1] && (Y == RoomHeight / 2 - 2 || Y == RoomHeight / 2 - 1 || Y == RoomHeight / 2  || Y == RoomHeight / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (Y == Height - 1 && Walls[2])
			{
				if (!(Doors[2] && (X == RoomWidth / 2 - 2 || X == RoomWidth / 2 - 1 || X == RoomWidth / 2  || X == RoomWidth / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (X == Width - 1 && Walls[3])
			{
				if (!(Doors[3] && (Y == RoomHeight / 2 - 2 || Y == RoomHeight / 2 - 1 || Y == RoomHeight / 2  || Y == RoomHeight / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			TileInfo.PackedTileIndex = FMath::RandRange(0, 31);
			SetTile(X, Y, 1, TileInfo);
		}
	}

	
	SetLayerCollision(0, true, true, 100);
	SetLayerCollision(1, true, true, 1);
	*/
	
}

