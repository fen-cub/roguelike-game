// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameState.h"
#include "Kismet/GameplayStatics.h"
#include "TestPlayerState.h"

void ATestGameState::GameOver(bool bIsWon) const {
   // Cast<ATestPlayerState>(PlayerArray[0])->GameOver(bIsWon);
}

bool ATestGameState::OpenNextLevel()
{
    if (LevelNumber == 3) {
        GameOver(true);
        return false;
    } else {
        if (LevelNumber == 1) {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("SecondLevel"));
        } else {
            UGameplayStatics::OpenLevel(GetWorld(), TEXT("ThirdLevel"));
        }
        return true;
    }
}