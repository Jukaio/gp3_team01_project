// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/FG_LocomotionState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_SFSM.h"

// Sets default values for this component's properties
UFG_LocomotionState::UFG_LocomotionState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFG_LocomotionState::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFG_LocomotionState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFG_LocomotionState::OnStateTick_Implementation(float DeltaTime)
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

	Locomotion->Hover(Stats->HoverStats);
	Locomotion->CheckTouching(Primitive);

	StateMachine->OnStateMachineTick(DeltaTime);
	if (Stats->InputStats->GetWasJumpingJustPressed()) 
	{
		Locomotion->Jump(Stats->JumpStats);
	}

	return true;
}
