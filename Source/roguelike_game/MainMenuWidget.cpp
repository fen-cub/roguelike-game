// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

/*
void UMainMenuWidget::NativeConstruct() {
    Super::NativeConstruct();

    HostOnlineGameButton->OnClicked.AddDynamic(
        this, &ThisClass::UMainMenuWidget::OnHostOnlineGameButtonClicked);

    //GetMyGameSubsystem()->OnCreateSessionCompleteEvent.AddDynamic(
    //    this, &ThisClass::StartSessionWhenCreatingSessonComplete);
}

void UMainMenuWidget::OnHostOnlineGameButtonClicked() {
    // Ensure we have left any session
    //GetMyGameSubsystem()->DestroySession();
    
    //GetMyGameSubsystem()->CreateSession(
    //    "Lobby " + FString::FromInt(FMath::RandRange(1, 1e6)), 2, true);
}

void UMainMenuWidget::StartSessionWhenCreatingSessonComplete(bool bSuccess) {
    if (!bSuccess) {
        return;
    }
    //GetMyGameSubsystem()->StartSession();
}


USessionsGameInstanceSubsystem *UMainMenuWidget::GetMyGameSubsystem() const {
    const UGameInstance *GameInstance = UGameplayStatics::GetGameInstance(
        GetWorld());
    USessionsGameInstanceSubsystem *GameInstanceSubsystem = GameInstance->
        GetSubsystem<USessionsGameInstanceSubsystem>();
    return GameInstanceSubsystem;
}
*/
