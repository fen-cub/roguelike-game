// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RoomActor.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultRoom.generated.h"

UCLASS()
class ROGUELIKE_GAME_API ADefaultRoom : public ARoomActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ADefaultRoom(const TArray<bool> Doors);

};
