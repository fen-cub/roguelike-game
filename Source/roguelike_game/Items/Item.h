// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components\CapsuleComponent.h"
#include "../Interfaces/InteractableInterface.h"
#include "PaperSpriteActor.h"
#include "Item.generated.h"

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AItem> Class;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UTexture2D* Image{nullptr};
	
	bool IsEmpty() const;
};

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AItem : public APaperSpriteActor, public IInteractableInterface
{
	GENERATED_BODY()

	AItem();

protected:
	// The main skeletal mesh associated with this item.
	UPROPERTY(Category = Item, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UPaperFlipbookComponent* Sprite;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Trigger)
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(EditAnywhere, Category = Config)
	FItemData Data;

public:
	// Flipbook texture of item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* ItemFlipbook{nullptr};

	virtual void Interact(class APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	FItemData GetItemData() const;
};
