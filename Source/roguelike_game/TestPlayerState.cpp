// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPlayerState.h"

void ATestPlayerState::GameOver_Implementation(bool bIsWon) {
    UGameOverWidget *CreatedWidget = CreateWidget<UGameOverWidget>(
        GetWorld(), GameOverWidgetClass);
    UE_LOG(LogTemp, Warning, TEXT("In Game Over creating Widget"))
    CreatedWidget->AddToViewport();
    CreatedWidget->SetWidgetText(bIsWon);
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController) {
        PlayerController->SetShowMouseCursor(true);
    }
}