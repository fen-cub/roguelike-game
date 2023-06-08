// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapComponent.h"
#include "Room.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API URoom : public UPaperTileMapComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Doors")
	TArray<bool> Doors;

	uint8 RoomWidth;
	uint8 RoomHeight;

	TArray<bool> Walls;
	
	void CreateRoom(const uint8 Width, const uint8 Height, const uint8 Side, int TemplateNum = -1);
};
