// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/FG_AirborneState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"

UFG_AirborneState::UFG_AirborneState()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFG_AirborneState::BeginPlay()
{
	Super::BeginPlay();
}

void UFG_AirborneState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UFG_AirborneState::OnStateTick_Implementation(float DeltaTime)
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

	//Locomotion->CheckTouching(Primitive);

	//Prepare data

	Locomotion->MoveWithFriction(Stats->InputStats, Stats->AirborneStats);
	
	if (Locomotion->Hover(Stats->HoverStats)) //Hover ray basically acts as a ground check
	{
		return false;
	}
    return true;
}
