// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Locomotion/FG_LocomotionIdleSubState.h"
#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"

// Sets default values for this component's properties
UFG_LocomotionPlayerState::UFG_LocomotionPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Initialise SubStates
	LocomotionIdleSubState = CreateDefaultSubobject<UFG_LocomotionIdleSubState>(TEXT("LocomotionIdleState"));
	LocomotionMovingSubState = CreateDefaultSubobject<UFG_LocomotionMovingSubState>(TEXT("LocomotionMovingState"));
	LocomotionJumpingSubState = CreateDefaultSubobject<UFG_LocomotionJumpingSubState>(TEXT("LocomotionJumpingState"));
	LocomotionAirborneSubState = CreateDefaultSubobject<UFG_LocomotionAirborneSubState>(TEXT("LocomotionAirborneState"));
	// ...
}


// Called when the game starts
void UFG_LocomotionPlayerState::BeginPlay()
{
	Super::BeginPlay();
	SubStateMachine = NewObject<UFG_SFSM>();
	SubStateMachine->Push(LocomotionIdleSubState);
	// ...
}


// Called every frame
void UFG_LocomotionPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFG_LocomotionPlayerState::OnStateTick_Implementation(float DeltaTime)
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

	Locomotion->CheckTouching(Primitive);
	SubStateMachine->OnStateMachineTick(DeltaTime);

	return true;
}
