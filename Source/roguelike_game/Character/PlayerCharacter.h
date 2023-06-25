// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "roguelike_game/Components/CharacterAnimationComponent.h"
#include "roguelike_game/Widgets/PlayerHUD.h"
#include "EnvironmentQuery/EnvQueryDebugHelpers.h"
#include "PlayerCharacter.generated.h"

/*
The character controlled by the player during the game process.
 */
UCLASS()
class ROGUELIKE_GAME_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Set default player properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsAttacking;

protected:
	// True if character is dead
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRepSetDying, Category = "State")
	bool bIsDead;

	// True if character is moving
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "MovementCharacter | Config")
	bool bIsMoving;

	// Max sprint speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "MovementCharacter | Config")
	float SprintSpeed;

	// Max walking speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "MovementCharacter | Config")
	float WalkSpeed;

	// Epsilon for float types comparison
	float ComparisonErrorTolerance;

	DECLARE_DELEGATE_OneParam(FNumberKeyActionDelegate, const int64);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	class UCharacterAnimationComponent* AnimationComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger)
	class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	class UItemStorageComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Equipment)
	class UItemStorageComponent* EquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UCharacterAttributesComponent* AttributesComponent;

	// Sets up on the BeginPlay()
	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

	UPROPERTY()
	class AStorage* InteractableStorage;

	// How much Stamina regenerates for a Tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float StaminaRegenerateRate;

	// How much Health regenerates for a Tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float HealthRegenerateRate;

	// How much Stamina losses for a Tick while running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float RunningStaminaLossRate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

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

	UFUNCTION()
	void SwitchMouseCursorVisibility();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

	// Moves character forward or down
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void MoveForwardOrDown(const float Axis);

	// Moves character right or left
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void MoveRightOrLeft(const float Axis);

	// Increases maximum speed
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void Sprint();

	// Sets maximum speed to default  
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void StopSprint();

	// Interact with Interactable Interface
	UFUNCTION(BlueprintCallable, Category= Trigger)
	void UseItem(const int64 Position);

	// Stops movements and calls death animation
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void Die();

private:
	UFUNCTION(Server, Reliable)
	void ServerSetDying();

	UFUNCTION()
	void OnRepSetDying();

protected:
	// Interact with Interactable Interface
	UFUNCTION(BlueprintCallable, Category= Trigger)
	void Interact();

private:
	UFUNCTION(Server, Reliable)
	void ServerInteract();

	UFUNCTION(NetMulticast, Reliable)
	void OnRep_Interact(int64 RandomHash);

protected:
	// Attack enemy
	UFUNCTION()
	void Attack();

private:
	UFUNCTION(Server, UnReliable)
	void ServerAttack();

	UFUNCTION(NetMulticast, Unreliable)
	void OnRep_Attack();

	UFUNCTION(Server, Unreliable)
	void ServerSetMaxWalkSpeed(float NewMaxWalkSpeed);

	UFUNCTION(NetMulticast, Unreliable)
	void OnRep_SetMaxWalkSpeed(float NewMaxWalkSpeed);

	UFUNCTION(Server, Unreliable)
	void ServerSetWalkSpeed(float NewWalkSpeed);

	UFUNCTION(Server, Unreliable)
	void ServerSetSprintSpeed(float NewSprintSpeed);

public:
	// Called every tick locally
	virtual void Tick(float DeltaSeconds) override;

	UItemStorageComponent* GetInventoryComponent() const;

	UItemStorageComponent* GetEquipmentComponent() const;

	UCharacterAttributesComponent* GetAttributesComponent() const;

	UPlayerHUD* GetPlayerHUD() const;

	UFUNCTION(BlueprintCallable)
	AStorage* GetInteractableStorage() const;

	UFUNCTION(BlueprintCallable)
	void SetInteractableStorage(AStorage* const NewInteractableStorage);

	UFUNCTION(BlueprintCallable)
	void SetMaxWalkSpeed(float NewMaxWalkSpeed);

	float GetSprintSpeed() const;

	UFUNCTION(BlueprintCallable)
	void SetSprintSpeed(const float NewSprintSpeed);

	float GetWalkSpeed() const;

	UFUNCTION(BlueprintCallable)
	void SetWalkSpeed(const float NewWalkSpeed);
};
