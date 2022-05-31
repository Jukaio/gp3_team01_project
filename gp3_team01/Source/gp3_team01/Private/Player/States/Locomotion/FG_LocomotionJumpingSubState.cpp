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
	bWasJustPushed = true;
	Timer = 0.0f;
}

bool UFG_LocomotionJumpingSubState::OnStateTick_Implementation(float DeltaTime)
{
	if (bWasJustPushed == false) {
		return false;
	}
	UFG_DA_InputStats* InputStats = FGPlayerCharacter->Stats->InputStats;
	if (bWasJustPushed && InputStats->GetIsJumpingPressed()) {
		UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
		const FVector Input = InputStats->GetCameraNormalisedInputVector();
		if (!Input.IsNearlyZero())
		{
			Locomotion->Move(InputStats, FGPlayerCharacter->Stats->AirborneStats);
		}
		Locomotion->ApplyFriction(FGPlayerCharacter->Stats->AirborneStats);
		Locomotion->RotatePlayerRAW(FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector(), 15.f);
		
		const UFG_DA_JumpStats* JumpStats = FGPlayerCharacter->Stats->JumpStats;
		const FRichCurveKey LastKey = JumpStats->ForceCurve->FloatCurve.GetLastKey();

		if (!(Timer > LastKey.Time)) {
			float Force = FGPlayerCharacter->Stats->JumpStats->JumpForce;

			Force = FMath::Lerp(0.0f, Force, JumpStats->ForceCurve->GetFloatValue(Timer));

			Locomotion->JumpRAW(Force, JumpStats->RayDistance, JumpStats->DisableFloatingTime);
			Timer += DeltaTime;
			return true;			
		}
	}

	Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
	bWasJustPushed = false;
	return true;
}
