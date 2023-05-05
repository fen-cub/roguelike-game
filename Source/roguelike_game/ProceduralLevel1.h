// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomActor.h"
#include "GameFramework/Actor.h"
#include "ProceduralLevel1.generated.h"

UCLASS()
class ROGUELIKE_GAME_API AProceduralLevel1 : public AActor
{
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
	
	ARoomActor* CreateRoomWithDoors() const;

	ARoomActor* CreateRoomWithDir(uint8 Direction, FVector PrevLocation) const;

	uint8 NumOfRooms;

};
