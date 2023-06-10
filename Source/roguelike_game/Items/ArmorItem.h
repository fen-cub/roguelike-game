// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ArmorItem.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AArmorItem : public AItem
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	bool bIsEquipped = false;
	
public:
	
	virtual void Use(class APlayerCharacter* PlayerCharacter) override;
	
};
