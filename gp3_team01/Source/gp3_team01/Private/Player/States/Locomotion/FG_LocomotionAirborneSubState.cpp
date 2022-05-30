// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"

bool UFG_LocomotionAirborneSubState::OnStateTick_Implementation(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage
	(
		INDEX_NONE,
		DeltaTime,
		FColor::Magenta,
		TEXT("I AM RUNNIN!")
	);

	//Input
	const UFG_DA_InputStats* InputStats = FGPlayerCharacter->Stats->InputStats;
	const FVector Input = InputStats->GetCameraNormalisedInputVector();
	if (InputStats->GetIsGlidingPressed()) 
	{
		FGPlayerCharacter->StateMachine->Push(FGPlayerCharacter->GlidingCoreState);
		return true;
	}

	//Apply movement
	if (!Input.IsNearlyZero()) 
	{
		FGPlayerCharacter->LocomotionComp->Move(FGPlayerCharacter->Stats->InputStats, FGPlayerCharacter->Stats->AirborneStats);
	}
	FGPlayerCharacter->LocomotionComp->ApplyFriction(FGPlayerCharacter->Stats->AirborneStats);

	//you should pop yourself NOW - LTG
	const bool IsGrounded = FGPlayerCharacter->LocomotionComp->Hover(FGPlayerCharacter->Stats->HoverStats);
	return !IsGrounded;
}
