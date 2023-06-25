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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UInventory* InventoryWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	class UEquipmentWidget* EquipmentWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UStorageDisplay* InteractableStorageWidget;

public:
	UStorageDisplay* GetInteractableStorageWidget() const;

	void SetInteractableStorageWidget(UStorageDisplay* const NewInteractableStorageWidget);

	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetStamina(float CurrentStamina, float MaxStamina);

	UProgressBar* GetHealthBar() const;

	UProgressBar* GetStaminaBar() const;

	UInventory* GetInventoryWidget() const;

	UEquipmentWidget* GetEquipmentWidget() const;
};
