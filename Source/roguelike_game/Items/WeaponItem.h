// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponItem.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AWeaponItem : public AItem
{
	GENERATED_BODY()

public:
	virtual void Use(class APlayerCharacter* PlayerCharacter, int64 InventoryPosition) override;
};
