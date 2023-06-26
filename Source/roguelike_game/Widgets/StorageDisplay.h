// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StorageDisplay.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UStorageDisplay : public UUserWidget
{
	GENERATED_BODY()

public:
	UStorageDisplay(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UInventory* InventoryWidget;

public:
	UInventory* GetInventoryWidget() const;
};
