// Fill out your copyright notice in the Description page of Project Settings.

#include "Room.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"

URoom::URoom()
{
	SetIsReplicated(true);
}


void URoom::CreateRoom(const uint8 Width, const uint8 Height, const uint8 Side, int TemplateNum)
{
	UE_LOG(LogTemp, Warning, TEXT("In Create Room %d"), TemplateNum)
	//UE_LOG(LogTemp, Warning, TEXT("TemplNum %d"), TemplateNum)
	RoomWidth = Width;
	RoomHeight = Height;
	CreateNewTileMap(Width, Height, 16, 16, 1.0f);
	AddNewLayer();
	
	UPaperTileSet *TileSet = LoadObject<UPaperTileSet>(
	nullptr, TEXT("/Game/Textures/Tiles_TileSet"));

	FString FileName;
	
	UPaperTileMap *BaseMap = nullptr;
	
	if (Width == 6 && Height == 7)
	{
		FileName = FString::Printf(TEXT("/Game/RoomTemplates/VerticalCorridors/Template%d"), TemplateNum);
	} else if (Width == 7 && Height == 6)
	{
		FileName = FString::Printf(TEXT("/Game/RoomTemplates/HorizontalCorridors/Template%d"), TemplateNum);
	} else if (Width == 20 && Height == 7)
	{
		FileName = FString::Printf(TEXT("/Game/RoomTemplates/HorizontalAdditions/Template%d"), TemplateNum);
	} else if (Width == 7 && Height == 20)
	{
		FileName = FString::Printf(TEXT("/Game/RoomTemplates/VerticalAdditions/Template%d"), TemplateNum);
	} else
	{
		FileName = FString::Printf(TEXT("/Game/RoomTemplates/DefaultRooms/Template%d"), TemplateNum);
	}

	BaseMap = LoadObject<UPaperTileMap>(nullptr, *FileName);
	
	SetTileMap(BaseMap);
	
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
			TileInfo.PackedTileIndex = (!Walls[0] && !Walls[1]) ? 5 : 29;
			SetTile(0,1, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[0] && !Walls[1]) ? 29 : 5;
			SetTile(1,0, 1, TileInfo);
		}
		if ((Walls[1] || Doors[1]) && (Walls[2] || Doors[2]) || (!Walls[1] && !Walls[2]))
		{
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 77 : 153;
			SetTile(0 ,RoomHeight - 1, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 9 : 54;
			SetTile(1 ,RoomHeight - 2, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 55 : 29;
			SetTile(0,RoomHeight - 2, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[1] && !Walls[2]) ? 29 : 55;
			SetTile(1,RoomHeight - 1, 1, TileInfo);
		}
		if ((Walls[2] || Doors[2]) && (Walls[3] || Doors[3]) || (!Walls[2] && !Walls[3]))
		{
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 75 : 154;
			SetTile(RoomHeight - 1,RoomWidth - 1, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 7 : 56;
			SetTile(RoomHeight - 2,RoomWidth - 2, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 31 : 55;
			SetTile(RoomHeight - 2,RoomHeight - 1, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[2] && !Walls[3]) ? 55 : 31;
			SetTile(RoomHeight - 1, RoomHeight - 2, 1, TileInfo);
		}
		if ((Walls[3] || Doors[3]) && (Walls[0] || Doors[0]) || (!Walls[3] && !Walls[0]))
		{
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 100 : 104;
			SetTile(RoomWidth - 1, 0, 0, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 57 : 6;
			SetTile(RoomWidth - 2, 1, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 5 : 31;
			SetTile(RoomHeight - 1,1, 1, TileInfo);
			TileInfo.PackedTileIndex = (!Walls[3] && !Walls[0]) ? 31 : 5;
			SetTile(RoomHeight - 2,0, 1, TileInfo);
		}
	}
	
	SetLayerCollision(0, true, true, 100);
	
}

void URoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

