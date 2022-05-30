// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionIdleSubState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_SFSM.h"

bool UFG_LocomotionIdleSubState::OnStateTick_Implementation(float DeltaTime)
{
	const FVector Input = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	if (!Input.IsNearlyZero()) {
		Context->SubStateMachine->Push(Context->LocomotionMovingSubState);
		return true;
	}
	else if (FGPlayerCharacter->Stats->InputStats->GetWasJumpingJustPressed()) {
		Context->SubStateMachine->Push(Context->LocomotionJumpingSubState);
		return true;
	}
	if (!FGPlayerCharacter->LocomotionComp->Hover(FGPlayerCharacter->Stats->HoverStats)) {
		Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
		return true;
	}

	FGPlayerCharacter->LocomotionComp->ApplyFriction(FGPlayerCharacter->Stats->MoveStats);

	return true;
}
