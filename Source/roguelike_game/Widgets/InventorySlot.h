// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory.h"
#include "Blueprint/UserWidget.h"
#include "roguelike_game/Items/Item.h"
#include "InventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int64 PositionInInventory;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ItemButtonOnClicked();

	UFUNCTION()
	void InteractButtonOnClicked();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	FItemData ItemData;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UButton* ItemButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UButton* InteractButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UTextBlock* InteractButtonText;

	UFUNCTION(BlueprintCallable)
	void SetInteractButtonVisibility(ESlateVisibility NewVisibility);
};
