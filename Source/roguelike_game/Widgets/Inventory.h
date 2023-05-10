// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "roguelike_game/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API UInventory : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 Rows = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 Columns = 1;

	UFUNCTION(BlueprintCallable)
	int64 GetRow(int64 Position) const;
	
	UFUNCTION(BlueprintCallable)
	int64 GetColumn(int64 Position) const;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;

	UFUNCTION(BlueprintImplementableEvent)
	void InsertItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintCallable)
	void SetGridPanelSizes(int64 RowCount, int64 ColumnCount);
};
