// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_SFSM.h"
#include "Player/Movement/FG_LocomotionComponent.h"

void UFG_LocomotionJumpingSubState::OnStatePush_Implementation()
{
	FGPlayerCharacter->LocomotionComp->Jump(FGPlayerCharacter->Stats->JumpStats);
	Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
}

bool UFG_LocomotionJumpingSubState::OnStateTick_Implementation(float DeltaTime)
{
	return false;
}
