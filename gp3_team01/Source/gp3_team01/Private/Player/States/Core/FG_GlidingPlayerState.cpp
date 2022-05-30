// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Gliding/FG_GlidingIdleSubState.h"
#include "Player/States/Gliding/FG_GlidingSteeringSubState.h"

// Sets default values for this component's properties
UFG_GlidingPlayerState::UFG_GlidingPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	GlidingIdleSubState = CreateDefaultSubobject<UFG_GlidingIdleSubState>(TEXT("GlidingIdleState"));
	GlidingSteeringSubState = CreateDefaultSubobject<UFG_GlidingSteeringSubState>(TEXT("GlidingSteeringState"));
	// ...
}


// Called when the game starts
void UFG_GlidingPlayerState::BeginPlay()
{
	Super::BeginPlay();
	// ...
	SubStateMachine = NewObject<UFG_SFSM>();
}


// Called every frame
void UFG_GlidingPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_GlidingPlayerState::OnStateEnter_Implementation()
{
	SubStateMachine->Push(GlidingIdleSubState);


	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	CapsuleCollider->SetEnableGravity(false);
}

void UFG_GlidingPlayerState::OnStateExit_Implementation()
{
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;

	CapsuleCollider->SetEnableGravity(true);
}

bool UFG_GlidingPlayerState::OnStateTick_Implementation(float DeltaTime)
{
	SubStateMachine->OnStateMachineTick(DeltaTime);
	//UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
	return !SubStateMachine->IsEmpty();
}


