// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

	
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;
	
};
