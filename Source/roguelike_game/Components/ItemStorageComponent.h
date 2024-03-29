﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "roguelike_game/Widgets/Inventory.h"
#include "roguelike_game/Items/Item.h"
#include "ItemStorageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUELIKE_GAME_API UItemStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemStorageComponent();

	// Is the item content is random generated
	UPROPERTY(Replicated)
	bool bIsGenerated;

protected:
	// Storage and order of items
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	TArray<FItemData> ItemStorage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FItemData> ItemRandomStorage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 StorageSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	int64 FirstEmptySlotPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (InstanceEditable = "true", ExposeOnSpawn = "true"))
	FItemData EmptySlot;

	// Need to be set at the begining of the game
	UPROPERTY()
	class UInventory* InventoryWidget;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Setup properties that should be replicated from the server to clients.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = Storage)
	void ServerAddItem(FItemData Item, int64 Position);

	UFUNCTION(NetMulticast, Unreliable)
	void OnRep_AddItem(FItemData Item, int64 Position);

	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = Storage)
	void ServerRemoveItem(int64 Position);

	UFUNCTION(NetMulticast, Unreliable)
	void OnRep_RemoveItem(int64 Position);

	UFUNCTION(Server, Unreliable, BlueprintCallable, Category = Storage)
	void ServerUseItem(int64 Position);

	UFUNCTION(NetMulticast, Unreliable)
	void OnRep_UseItem(int64 Position);

	int64 GetNextRandomInteger(uint64 LastRandom) const;

	int64 GetRandomInRange(int64 Left, int64 Right, int64 RandomNumber) const;

public:
	UFUNCTION(BlueprintCallable, Category = Storage)
	void AddItem(FItemData Item, int64 Position);

	UFUNCTION(BlueprintCallable, Category = Storage)
	void RemoveItem(int64 Position);

	UFUNCTION(BlueprintCallable, Category = Storage)
	void UseItem(int64 Position);

	UFUNCTION()
	FItemData GetItem(int64 Position);

	void GenerateRandomContents(int64 RandomHash);

	// Called when HUD created
	void SetUpInventoryWidget(class UInventory* Widget);

	void SetStorageSize(int64 Size);

	int64 GetStorageSize() const;

	int64 GetFirstEmptySlotPosition() const;
};
