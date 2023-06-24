// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "roguelike_game/Character/PlayerCharacter.h"
#include "roguelike_game/Components/ItemStorageComponent.h"
#include "roguelike_game/Interfaces/InteractableInterface.h"
#include "Storage.generated.h"

/**
 The storage actor that contains the items. The player can add and remove items
 */
UCLASS()
class ROGUELIKE_GAME_API AStorage : public APaperSpriteActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	AStorage();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	class UItemStorageComponent* StorageComponent;

	UPROPERTY()
	class UStorageDisplay* StorageWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Tooltip)
	class UTextRenderComponent* Tooltip;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UStorageDisplay> StorageWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	class UCapsuleComponent* TriggerCapsule;

	// Sets spawn properties
	UFUNCTION()
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
					class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UItemStorageComponent* GetStorageComponent() const;

	virtual void Interact(class APlayerCharacter* PlayerCharacter) override;

	UFUNCTION(BlueprintCallable)
	void StopInteract(class APlayerCharacter* PlayerCharacter);
};
