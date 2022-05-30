// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/FG_GroundState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"

void UFG_GroundState::BeginPlay()
{
	Super::BeginPlay();
}

void UFG_GroundState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UFG_GroundState::OnStateTick_Implementation(float DeltaTime)
{
	if (FGPlayerCharacter == nullptr)
	{
		// Print Error here
		return false;
	}
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
	if (Locomotion == nullptr)
	{
		// Print Error here
		return false;
	}
	UPrimitiveComponent* Primitive = FGPlayerCharacter->Collider;
	if (Primitive == nullptr)
	{
		// Print Error here
		return false;
	}
	UFG_DA_PlayerStats* Stats = FGPlayerCharacter->Stats;
	if (Stats == nullptr)
	{
		// Print Error here
		return false;
	}

	
	Locomotion->MoveWithFriction(Stats->InputStats, Stats->MoveStats);
	if (Stats->InputStats->GetWasJumpingJustPressed())
	{
		Locomotion->Jump(Stats->JumpStats);
	}

	if (!Locomotion->Hover(Stats->HoverStats))
	{
		return false;
	}

	return true;
}
