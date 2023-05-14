// Fill out your copyright notice in the Description page of Project Settings.


#include "BigRoom.h"

// Sets default values
ABigRoom::ABigRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABigRoom::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABigRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

