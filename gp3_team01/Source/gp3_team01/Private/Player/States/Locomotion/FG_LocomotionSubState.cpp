// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_SFSM.h"

void UFG_LocomotionSubState::BeginPlay()
{
	Super::BeginPlay();
	Context = CastChecked<UFG_LocomotionPlayerState>(GetOwner()->GetComponentByClass(UFG_LocomotionPlayerState::StaticClass()));
}

UFG_LocomotionPlayerState* UFG_LocomotionSubState::GetContext() const
{
	return Context;
}
