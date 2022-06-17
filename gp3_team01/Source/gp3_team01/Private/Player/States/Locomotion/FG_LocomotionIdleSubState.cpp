// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionIdleSubState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "FG_BroadcastTemplate.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"

bool UFG_LocomotionIdleSubState::OnStateTick_Implementation(float DeltaTime)
{
	Super::OnStateTick_Implementation(DeltaTime);
	const FVector Input = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	FVector PlayerForward = FGPlayerCharacter->MeshPivot->GetForwardVector();
	PlayerForward.Z = 0.f;
	FQuat Rotation = PlayerForward.ToOrientationQuat();
	FGPlayerCharacter->MeshPivot->SetWorldRotation(Rotation);
	if (!Input.IsNearlyZero()) {
		Context->SubStateMachine->Push(Context->LocomotionMovingSubState);
		return true;
	}
	else if (FGPlayerCharacter->Stats->InputStats->GetWasJumpingJustPressed()) {
		Context->SubStateMachine->Push(Context->LocomotionJumpingSubState);
		return true;
	}
	if (!Context->IsCoyoteGrounded()) {
		Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
		return true;
	}

	if (FGPlayerCharacter->Stats->InputStats->GetWasGlidingJustPressed())
	{
		Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
		FGPlayerCharacter->LocomotionComp->JumpRAW(100.0f, FGPlayerCharacter->Stats->JumpStats->RayDistance);
		FGPlayerCharacter->StateMachine->Push(FGPlayerCharacter->GlidingCoreState);
		return true;
	}

	for (auto& Hit : Context->GroundHit)
	{
		BroadcastResponse<IFG_LocomotionResponse>(Hit, IFG_LocomotionResponse::Execute_OnLocomotionStand, FGPlayerCharacter->LocomotionComp);
	}
	FGPlayerCharacter->LocomotionComp->ApplyFriction(FGPlayerCharacter->Stats->MoveStats);

	return true;
}
