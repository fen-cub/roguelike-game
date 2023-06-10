// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "roguelike_game/Items/Item.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EInventoryType : uint8
{
	PlayerHUDInventory,
	PlayerInventoryInStorage,
	StorageInventory,
	EquipmentInventory
};

class UItemStorageComponent;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UUniformGridPanel* InventoryGridPanel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInventoryType CurrentInventoryType;

	UPROPERTY()
	UItemStorageComponent* OwnerStorage;
	
	UPROPERTY()
	UItemStorageComponent* PairingStorage;

	int64 LastClickedSlotPosition = -1;
	
public:
	UFUNCTION()
	EInventoryType GetCurrentInventoryType() const;

	UFUNCTION()
	void SetCurrentInventoryType(const EInventoryType NewInventoryType);

	UUniformGridPanel* GetInventoryGridPanel() const;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void InsertItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetItem(int64 Position, FItemData Item);
	
	UFUNCTION(BlueprintCallable)
	void SetGridPanelSizes(int64 RowCount, int64 ColumnCount);
	
	void SetOwnerStorage(UItemStorageComponent* const NewOwnerStorage);

	void SetPairingStorage(UItemStorageComponent* const NewPairingStorage);

	void SetNewClickedSlot(int64 Position);

	void HideLastClickedSlot();
	
	UItemStorageComponent* GetOwnerStorage() const;
	
	UItemStorageComponent* GetPairingStorage() const;
};
