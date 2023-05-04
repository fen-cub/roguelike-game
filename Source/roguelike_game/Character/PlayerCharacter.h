// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

// Possible character directions
UENUM(BlueprintType, Category = "AnimationCharacter | Animation")
enum class ECharacterDirection : uint8
{
	Up,
	Down,
	Left,
	Right
};

// Idle character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FIdleAnimationFlipbooks
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
};

// Walk character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FWalkingAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* WalkDown{nullptr};
};

// Run character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FRunningAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* RunDown{nullptr};
};

// Death character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FDeathAnimationFlipbooks
{
	GENERATED_BODY()

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

	// True if character is dead
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AnimationCharacter | Config")
	bool bIsMoving;

	// Epsilon for float types comparison
	const float ComparisonErrorTolerance = 1e-7;

	// Sets spawn settings
	virtual void BeginPlay() override;

	// Sets direction vector every moving
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	// Sets and binds input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Sets current character direction while moving
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void SetCurrentCharacterDirection(FVector const& Velocity);

	// Sets moving flipbooks if moving or idle otherwise
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateMovement(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	// Sets death animation and stops movement animations
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateDeath();

	// Moves character forward or down
	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void MoveForwardOrDown(const float Axis);

	// Moves character right or left
	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void MoveRightOrLeft(const float Axis);

	// Increases maximum speed
	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void Sprint();

	// Sets maximum speed to default  
	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void StopSprint();

	// Stops movements and calls death animation
	UFUNCTION(BlueprintCallable, Category="MovementCharacter | Movements")
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	// Current direction, default - Down
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	ECharacterDirection CurrentCharacterDirection;

	// Idle flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FIdleAnimationFlipbooks IdleFlipbooks;

	// Walking flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FWalkingAnimationFlipbooks WalkingFlipbooks;

	// Running flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FRunningAnimationFlipbooks RunningFlipbooks;

	// Death flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FDeathAnimationFlipbooks DeathFlipbooks;

public:
	virtual void Tick(float DeltaSeconds) override;
};
