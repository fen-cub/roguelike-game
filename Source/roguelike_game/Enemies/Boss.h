// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Boss.generated.h"

/**
 * 
 */
UCLASS()
class ROGUELIKE_GAME_API ABoss : public APaperCharacter
{
	GENERATED_BODY()

public:
	ABoss();
	UPROPERTY(EditAnywhere)
	class USphereComponent* DetectPlayerCollisionSphere;

	class USphereComponent* GetDetectPlayerCollisionSphere();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	USphereComponent* OverlapComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsDead, Category = "State")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "MovementCharacter | Config")
	bool bIsMoving;

	UPROPERTY(EditAnywhere, Category = "MovementCharacter | Config")
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Config")
	float DamageDealt;

	float ComparisonErrorTolerance;

	uint64 AttackTickCount = 0;
	uint64 DamageTickCount = 0;

	UFUNCTION()
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

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

	UFUNCTION(Server, Reliable)
	void ServerSetDying();

	UFUNCTION()
	void OnRep_IsDead();

private:
	virtual void Tick(float DeltaSeconds) override;
};
