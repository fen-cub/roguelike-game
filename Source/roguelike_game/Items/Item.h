// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components\CapsuleComponent.h"
#include "Components\TextRenderComponent.h"
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
	class UPaperSprite* Image{nullptr};
	
	bool IsEmpty() const;
};

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AItem : public APaperSpriteActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Trigger)
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tooltip)
	class UTextRenderComponent* Tooltip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	FItemData Data;

	// Setup properties that should be replicated from the server to clients.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Called when on Overlap
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Flipbook texture of item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* ItemFlipbook{nullptr};

	virtual void Interact(class APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	FItemData GetItemData() const;

	virtual void Use(class APlayerCharacter* PlayerCharacter);
};
