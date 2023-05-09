// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "roguelike_game/Items/Item.h"
#include "PlayerHUD.generated.h"

/**
  Player Head-Up-Display
  Health indicator + Stamina indicator
 */
UCLASS()
class ROGUELIKE_GAME_API UPlayerHUD : public UUserWidget
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
	
	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetStamina(float CurrentStamina, float MaxStamina);

	UFUNCTION(BlueprintImplementableEvent)
	void InsertItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintImplementableEvent)
	void SetItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintCallable)
	void SetGridPanelSizes(int64 RowCount, int64 ColumnCount);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;
};
