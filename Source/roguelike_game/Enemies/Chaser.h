// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "../Widgets/EnemyHealthBar.h"
#include "Chaser.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API AChaser : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Set default player properties
	AChaser();
	UPROPERTY(EditAnywhere)
	class USphereComponent* DetectPlayerCollisionSphere;

	class USphereComponent* GetDetectPlayerCollisionSphere();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* OverlapComponent;

protected:
	// True if character is dead
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsDead, Category = "State")
	bool bIsDead;


	// True if character is moving
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "MovementCharacter | Config")
	bool bIsMoving;

	// Max walking speed
	UPROPERTY(EditAnywhere, Category = "MovementCharacter | Config")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Config")
	float DamageDealt;

	// Epsilon for float types comparison
	float ComparisonErrorTolerance;

	uint64 AttackTickCount = 0;

	uint64 DamageTickCount = 0;

	// Sets spawn properties
	UFUNCTION()
	virtual void BeginPlay() override;

	// Sets spawn properties
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Setup properties that should be replicated from the server to clients.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Sets direction vector every moving
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	// Sets and binds input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category= "AnimationCharacter | Config")
	void UpdateMovementProperties(float DeltaTime, FVector OldLocation, FVector const OldVelocity);

	// Moves character forward or down
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void MoveForwardOrDown(const float Axis);

	// Moves character right or left
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void MoveRightOrLeft(const float Axis);

	// Stops movements and calls death animation
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void Die();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	class UCharacterAnimationComponent* AnimationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UCharacterAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float Health;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float MaxHealth;
	
	UPROPERTY(EditAnywhere, Category = Attributes)
	float StaminaRegenerateRate;

	UPROPERTY(EditAnywhere, Category = Attributes)
	float RunningStaminaLossRate;

	// Calls server to set dying
	UFUNCTION(Server, Reliable)
	void ServerSetDying();

	// Calls back from server to set dying
	UFUNCTION()
	void OnRep_IsDead();

private:
	// Called every tick locally
	virtual void Tick(float DeltaSeconds) override;
};
