// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ArtifactItem.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AArtifactItem : public AItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ApplyArtifactProperties(class APlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RemoveArtifactProperties(class APlayerCharacter* PlayerCharacter);

	virtual void Use(class APlayerCharacter* PlayerCharacter, int64 InventoryPosition) override;
};
