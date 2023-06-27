// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UGameOverWidget::SetWidgetText_Implementation(bool HasWon) {
    if (HasWon) {
        GameOverText->SetText(FText::FromString("You won!"));
    } else {
        GameOverText->SetText(FText::FromString("You died!"));
    }
}