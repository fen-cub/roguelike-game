// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbook.h"

#include "CharacterAnimationComponent.generated.h"

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

// Attack character animations flipbooks
USTRUCT(BlueprintType, Category = "Animation")
struct FAttackAnimationFlipbooks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* AttackUp{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* AttackRight{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* AttackLeft{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPaperFlipbook* AttackDown{nullptr};
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

/**
 Component sets current animation depending om direction
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROGUELIKE_GAME_API UCharacterAnimationComponent : public UPaperSpriteComponent
{
	GENERATED_BODY()

protected:
	// Epsilon for float types comparison
	float ComparisonErrorTolerance;

	// Current direction, default - Down
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	ECharacterDirection CurrentCharacterDirection;

	// Sprite of owner
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	class UPaperFlipbookComponent* OwnerFlipbookComponent;

	// Idle flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FIdleAnimationFlipbooks IdleFlipbooks;

	// Walking flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FWalkingAnimationFlipbooks WalkingFlipbooks;

	// Running flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FRunningAnimationFlipbooks RunningFlipbooks;

	// Attack flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FAttackAnimationFlipbooks AttackFlipbooks;

	// Death flipbooks animation storage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationCharacter | Config")
	FDeathAnimationFlipbooks DeathFlipbooks;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnFinishedAttack();

public:
	UCharacterAnimationComponent();

	// Sets owner character
	UFUNCTION(BlueprintCallable, Category="Config")
	void SetupOwner(UPaperFlipbookComponent* FlipbookComponent);

	// Sets current character direction while moving
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void SetCurrentCharacterDirection(FVector const& Velocity);

	// Sets walking flipbooks  
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateWalking();

	// Sets running flipbooks
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateRunning();

	// Sets animation flipbooks 
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateIdle();

	// Sets attack animation 
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateAttack();

	// Sets death animation 
	UFUNCTION(BlueprintCallable, Category="AnimationCharacter | Animation")
	void AnimateDeath();
};
