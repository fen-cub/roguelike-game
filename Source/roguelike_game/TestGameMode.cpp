// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "TestGameState.h"
#include "TestPlayerState.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"

ATestGameMode::ATestGameMode() {
    UE_LOG(LogTemp, Warning, TEXT("GameMode Constructor"));
    GameStateClass = ATestGameState::StaticClass();
    PlayerStateClass = ATestPlayerState::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();
}

auto ATestGameMode::GetMyGameState() const {
    return GetGameState<ATestGameState>();
}

void ATestGameMode::BeginPlay() {
    Super::BeginPlay();
    switch (LevelNumber)
    {
    case 1:
        GetWorld()->SpawnActor<ALevel1>(FVector(0.f), FRotator(0.f));
        break;
    case 2:
        GetWorld()->SpawnActor<ALevel2>(FVector(0.f), FRotator(0.f));
        break;
    case 3:
        GetWorld()->SpawnActor<ALevel3>(FVector(0.f), FRotator(0.f));
        break;
    }
    GetMyGameState()->LevelNumber = LevelNumber;
    UE_LOG(LogTemp, Warning, TEXT("LevelNumber: %d"), GetMyGameState()->LevelNumber);
}

void ATestGameMode::PostLogin(APlayerController *NewPlayer) {
    Super::PostLogin(NewPlayer);
    NewPlayer->SetShowMouseCursor(false);
    UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
}