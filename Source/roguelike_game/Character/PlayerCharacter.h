// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

/**
 test description
 */

UENUM(BlueprintType, Category = "AnimationCharacter|Animation")
enum class EAnimationDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleDown{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkDown{nullptr};
};

UCLASS()
class ROGUELIKE_GAME_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDead;

protected:
	virtual void BeginPlay() override;

	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="AnimationCharacter|Animation")
	void SetCurrentAnimationDirection(FVector const& Velocity);

	UFUNCTION(BlueprintCallable, Category="AnimationCharacter|Animation")
	void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
	EAnimationDirection CurrentAnimationDirection{EAnimationDirection::Down};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter|Config")
	FAnimationFlipbooks Flipbooks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter|Config")
	uint8 bIsMoving : 1;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

public:
	virtual void Tick(float DeltaSeconds) override;
};
