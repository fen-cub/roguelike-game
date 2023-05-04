// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

// Possible character directions
UENUM(BlueprintType, Category = "AnimationCharacter | Animation")
enum class EAnimationDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

// All character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* IdleDown{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkDown{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunDown{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* DieRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* DieLeft{nullptr};
};

/*
The character controlled by the player during the game process.
 */

UCLASS()
class ROGUELIKE_GAME_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Set default player settings
	APlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter | Config")
	uint8 bIsMoving : 1;

	// Epsilon for float types comparison
	const float ComparisonErrorTolerance = 1e-7;

	// Called when spawned
	virtual void BeginPlay() override;

	// Called when moves
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Called every animation while alive
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void SetCurrentAnimationDirection(FVector const& Velocity);

	// Called while alive or dying
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void Animate(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void MoveForwardOrDown(const float Axis);

	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void MoveRightOrLeft(const float Axis);

	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void Sprint();

	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void StopSprint();

	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	EAnimationDirection CurrentAnimationDirection{EAnimationDirection::Down};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FAnimationFlipbooks Flipbooks;

public:
	virtual void Tick(float DeltaSeconds) override;
};
