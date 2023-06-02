// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Components/CharacterAnimationComponent.h"
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

protected:
	// True if character is dead
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_IsDead, Category = "State")
	bool bIsDead;

	// True if character is moving
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "MovementCharacter | Config")
	bool bIsMoving;

	// True if character is sprinting
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_IsSprinting,
		Category = "MovementCharacter | Config")
	bool bIsSprinting;

	// Max sprint speed
	UPROPERTY(EditAnywhere, Category = "MovementCharacter | Config")
	float SprintSpeed;

	// Max walking speed
	UPROPERTY(EditAnywhere, Category = "MovementCharacter | Config")
	float WalkSpeed;

	// Epsilon for float types comparison
	float ComparisonErrorTolerance;

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

	// Increases maximum speed
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void Sprint();

	// Sets maximum speed to default  
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void StopSprint();

	// Stops movements and calls death animation
	UFUNCTION(BlueprintCallable, Category= "MovementCharacter | Movements")
	void Die();

	// Interact with Interactable Interface
	UFUNCTION(BlueprintCallable, Category= Trigger)
	void Interact();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	UFUNCTION(NetMulticast, Reliable)
	void OnRep_Interact();

	// Interact with Interactable Interface
	UFUNCTION(BlueprintCallable, Category= Trigger)
	void UseItem(const float Axis);

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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attributes)
	class UCharacterAttributesComponent* AttributesComponent;

	// Sets up on the BeginPlay()
	UPROPERTY()
	class UPlayerHUD* PlayerHUD;

public:
	UPlayerHUD* GetPlayerHUD() const;

protected:
	// How much Stamina regenerates for a Tick
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float StaminaRegenerateRate;

	// How much Stamina losses for a Tick while running
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attributes)
	float RunningStaminaLossRate;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	// Set sprint state
	UFUNCTION()
	void SetSprinting(bool bNewSprinting);

	// Calls server to set sprinting
	UFUNCTION(Server, Reliable)
	void ServerSetSprinting(bool bNewSprinting);

	// Called back from server to set sprint
	UFUNCTION()
	void OnRep_IsSprinting();

	// Calls server to set dying
	UFUNCTION(Server, Reliable)
	void ServerSetDying();

	// Calls back from server to set dying
	UFUNCTION()
	void OnRep_IsDead();

	// Called when on Overlap
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
						class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);

public:
	// Called every tick locally
	virtual void Tick(float DeltaSeconds) override;
	
	UItemStorageComponent* GetInventoryComponent() const;

	UCharacterAttributesComponent* GetAttributesComponent() const;
	
};

