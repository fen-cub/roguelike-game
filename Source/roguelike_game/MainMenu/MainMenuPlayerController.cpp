// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "EditorDirectories.h"

void AMainMenuPlayerController::BeginPlay() {
    Super::BeginPlay();
    SetShowMouseCursor(true);
    UUserWidget *CreatedWidget = CreateWidget<UUserWidget>(
        GetWorld(), WidgetClass);
    CreatedWidget->AddToViewport();
}