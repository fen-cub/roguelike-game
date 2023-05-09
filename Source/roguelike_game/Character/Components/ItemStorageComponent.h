// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "roguelike_game/HUD/PlayerHUD.h"
#include "roguelike_game/Items/Item.h"
#include "ItemStorageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROGUELIKE_GAME_API UItemStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemStorageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Storage and order of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	TArray<FItemData> ItemStorage;

	// Storage and order of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 StorageSize;
	
	// Storage and order of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 FirstEmptySlotPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	FItemData EmptySlot;
	
	// Need to be set at the begining of the game
	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

public:
	UFUNCTION(BlueprintCallable, Category = Storage)
	void AddItem(FItemData Item);
	
	UFUNCTION(BlueprintCallable, Category = Storage)
	void RemoveItem(int64 Position);

	UFUNCTION(BlueprintCallable, Category = Storage)
    void UseItem(int64 Position);

	// Called when HUD created
	void SetUpHUD(class UPlayerHUD* HUD);
	
	void SetStorageSize(int64 Size);
};
