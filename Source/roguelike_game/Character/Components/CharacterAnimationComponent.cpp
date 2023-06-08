// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimationComponent.h"
#include "PaperFlipbookComponent.h"
#include "roguelike_game/Character/PlayerCharacter.h"

UCharacterAnimationComponent::UCharacterAnimationComponent()
{
	CurrentCharacterDirection = ECharacterDirection::Down;
	ComparisonErrorTolerance = 1e-7;
}
void UCharacterAnimationComponent::SetupOwner(UPaperFlipbookComponent* FlipbookComponent)
{
	OwnerFlipbookComponent = FlipbookComponent;
	AnimateIdle();
}

// Called while walking
void UCharacterAnimationComponent::AnimateWalking()
{
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		OwnerFlipbookComponent->SetFlipbook(WalkingFlipbooks.WalkUp);
		break;
	case ECharacterDirection::Left:
		OwnerFlipbookComponent->SetFlipbook(WalkingFlipbooks.WalkLeft);
		break;
	case ECharacterDirection::Right:
		OwnerFlipbookComponent->SetFlipbook(WalkingFlipbooks.WalkRight);
		break;
	case ECharacterDirection::Down:
		OwnerFlipbookComponent->SetFlipbook(WalkingFlipbooks.WalkDown);
		break;
	default:
		break;
	}
}

// Called while running
void UCharacterAnimationComponent::AnimateRunning()
{
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		OwnerFlipbookComponent->SetFlipbook(RunningFlipbooks.RunUp);
		break;
	case ECharacterDirection::Left:
		OwnerFlipbookComponent->SetFlipbook(RunningFlipbooks.RunLeft);
		break;
	case ECharacterDirection::Right:
		OwnerFlipbookComponent->SetFlipbook(RunningFlipbooks.RunRight);
		break;
	case ECharacterDirection::Down:
		OwnerFlipbookComponent->SetFlipbook(RunningFlipbooks.RunDown);
		break;
	default:
		break;
	}
}

// Called while idle
void UCharacterAnimationComponent::AnimateIdle()
{
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		OwnerFlipbookComponent->SetFlipbook(IdleFlipbooks.IdleUp);
		break;
	case ECharacterDirection::Down:
		OwnerFlipbookComponent->SetFlipbook(IdleFlipbooks.IdleDown);
		break;
	case ECharacterDirection::Left:
		OwnerFlipbookComponent->SetFlipbook(IdleFlipbooks.IdleLeft);
		break;
	case ECharacterDirection::Right:
		OwnerFlipbookComponent->SetFlipbook(IdleFlipbooks.IdleRight);
		break;
	default:
		break;
	}
}

void UCharacterAnimationComponent::AnimateAttack()
{
	OwnerFlipbookComponent->SetLooping(false);
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		OwnerFlipbookComponent->SetFlipbook(AttackFlipbooks.AttackUp);
		break;
	case ECharacterDirection::Down:
		OwnerFlipbookComponent->SetFlipbook(AttackFlipbooks.AttackDown);
		break;
	case ECharacterDirection::Left:
		OwnerFlipbookComponent->SetFlipbook(AttackFlipbooks.AttackLeft);
		break;
	case ECharacterDirection::Right:
		OwnerFlipbookComponent->SetFlipbook(AttackFlipbooks.AttackRight);
		break;
	default:
		break;
	}
	OwnerFlipbookComponent->OnFinishedPlaying.AddDynamic(this, &UCharacterAnimationComponent::SetLoopingOnFinishedPlaying);
}

// Called when dying
void UCharacterAnimationComponent::AnimateDeath()
{
	OwnerFlipbookComponent->SetLooping(false);
	switch (CurrentCharacterDirection)
	{
	case ECharacterDirection::Up:
		OwnerFlipbookComponent->SetFlipbook(DeathFlipbooks.DieRight);
		break;
	case ECharacterDirection::Down:
		OwnerFlipbookComponent->SetFlipbook(DeathFlipbooks.DieLeft);
		break;
	case ECharacterDirection::Left:
		OwnerFlipbookComponent->SetFlipbook(DeathFlipbooks.DieLeft);
		break;
	case ECharacterDirection::Right:
		OwnerFlipbookComponent->SetFlipbook(DeathFlipbooks.DieRight);
		break;
	default:
		break;
	}
}

void UCharacterAnimationComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterAnimationComponent::SetLoopingOnFinishedPlaying()
{
	UE_LOG(LogTemp, Warning, TEXT("On set looping"));
	OwnerFlipbookComponent->SetLooping(true);
	UE_LOG(LogTemp, Warning, TEXT("%d"), OwnerFlipbookComponent->IsLooping());
	
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (PlayerCharacter)
	{
		PlayerCharacter->bIsAttacking = false;	
	}

	OwnerFlipbookComponent->OnFinishedPlaying.RemoveDynamic(this, &UCharacterAnimationComponent::SetLoopingOnFinishedPlaying);
}


// Called every animation while alive
void UCharacterAnimationComponent::SetCurrentCharacterDirection(FVector const& Velocity)
{
	const float x = Velocity.GetSafeNormal().X;
	const float y = Velocity.GetSafeNormal().Y;

	bool bIsMoving = !FMath::IsNearlyZero(Velocity.Size(), ComparisonErrorTolerance);

	if (bIsMoving)
	{
		if (y > 0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Right;
		}
		else if (y < -0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Left;
		}
		else if (x < -0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Down;
		}
		else if (x > 0.5f)
		{
			CurrentCharacterDirection = ECharacterDirection::Up;
		}
	}
}
