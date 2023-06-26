// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DisplayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDisplayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROGUELIKE_GAME_API IDisplayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetHealth(float CurrentHealth, float MaxHealth) PURE_VIRTUAL(IInteractableInterface::Interact,);

	virtual void SetStamina(float CurrentHealth, float MaxHealth) PURE_VIRTUAL(IInteractableInterface::Interact,);
};
