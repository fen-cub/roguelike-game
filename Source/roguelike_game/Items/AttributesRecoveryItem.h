// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "AttributesRecoveryItem.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AAttributesRecoveryItem : public AItem
{
	GENERATED_BODY()

public:
	AAttributesRecoveryItem();
	
protected:

	UPROPERTY(EditAnywhere, Category = Config)
	float HealthRecoveryValue;

	UPROPERTY(EditAnywhere, Category = Config)
	float StaminaRecoveryValue;

public:

	virtual void Use(class APlayerCharacter* PlayerCharacter, int64 InventoryPosition) override;
};
