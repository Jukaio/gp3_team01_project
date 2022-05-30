// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"

bool UFG_LocomotionMovingSubState::OnStateTick_Implementation(float DeltaTime)
{
	const FVector Input = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	
	if (FGPlayerCharacter->Stats->InputStats->GetWasJumpingJustPressed()) {
		Context->SubStateMachine->Push(Context->LocomotionJumpingSubState);
		return true;
	}
	
	FGPlayerCharacter->LocomotionComp->Move(FGPlayerCharacter->Stats->InputStats, FGPlayerCharacter->Stats->MoveStats);
	FGPlayerCharacter->LocomotionComp->ApplyFriction(FGPlayerCharacter->Stats->MoveStats);
	
	//If hover ray doesnt hit, go airborne!
	if (!FGPlayerCharacter->LocomotionComp->Hover(FGPlayerCharacter->Stats->HoverStats))
	{
		Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
		return true;
	}

	if (!Input.IsNearlyZero()) {

		return true;
	}

	return false;
}
