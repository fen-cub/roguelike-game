// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "PaperTileSet.h"

// Rotation -90 180 180

void URoom::SetDoors(TArray<bool> DoorsInfo)
{
	Doors = MoveTemp(DoorsInfo);
}

TArray<bool> URoom::GetDoors()
{
	return Doors;
}

void URoom::CreateRoom(const uint8 Width, const uint8 Height)
{
	RoomWidth = Width;
	RoomHeight = Height;
	CreateNewTileMap(Width, Height, 32, 32, 2.0f);
	AddNewLayer();
	
	UPaperTileSet *TileSet = LoadObject<UPaperTileSet>(
	nullptr, TEXT("/Game/Textures/TX_Tileset_Grass_TileSet"));

	FPaperTileInfo TileInfo;
	TileInfo.TileSet = TileSet;

	for (int32 X = 0; X < Width; ++X) {
		for (int32 Y = 0; Y < Height; ++Y) {
			if (Y == 0)
			{
				if (!(Doors[0] && (X == RoomWidth / 2 - 2 || X == RoomWidth / 2 - 1 || X == RoomWidth / 2  || X == RoomWidth / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (X == 0)
			{
				if (!(Doors[1] && (Y == RoomHeight / 2 - 2 || Y == RoomHeight / 2 - 1 || Y == RoomHeight / 2  || Y == RoomHeight / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (Y == Height - 1)
			{
				if (!(Doors[2] && (X == RoomWidth / 2 - 2 || X == RoomWidth / 2 - 1 || X == RoomWidth / 2  || X == RoomWidth / 2 + 1))) {
					TileInfo.PackedTileIndex = 32;
					SetTile(X, Y, 0, TileInfo);
				}
			}
			if (X == Width - 1)
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

	SetLayerCollision(1, true, true, 1);
	SetLayerCollision(0, true, true, 100);
	
}

