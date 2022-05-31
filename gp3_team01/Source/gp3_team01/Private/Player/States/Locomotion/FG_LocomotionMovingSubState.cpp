// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"

void UFG_LocomotionMovingSubState::OnStateEnter_Implementation()
{
	Timer = 0.0f;
}

bool UFG_LocomotionMovingSubState::OnStateTick_Implementation(float DeltaTime)
{
	const FVector Input = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	
	if (FGPlayerCharacter->Stats->InputStats->GetWasJumpingJustPressed()) {
		Context->SubStateMachine->Push(Context->LocomotionJumpingSubState);
		return true;
	}

	UFG_DA_MoveStats* MoveStats = FGPlayerCharacter->Stats->MoveStats;
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;

	float Acceleration = MoveStats->AccelerationCurve ? MoveStats->AccelerationCurve->GetFloatValue(Timer) : 1.0f;
	Acceleration *= MoveStats->MaxAcceleration;
	Locomotion->MoveRAW(Input, Acceleration, MoveStats->MaxSpeed);
	Locomotion->ApplyFriction(MoveStats);

	float RotationSpeed = MoveStats->RotationCurve ? MoveStats->RotationCurve->GetFloatValue(Timer) : 1.0f;
	RotationSpeed *= MoveStats->MaxRotation;

	FVector ZInputDir = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	FGPlayerCharacter->LocomotionComp->RotatePlayerRAW(ZInputDir, RotationSpeed);

	const FRichCurveKey LastKey = MoveStats->AccelerationCurve->FloatCurve.GetLastKey();
	Timer += DeltaTime;
	Timer = FMath::Clamp(Timer, 0.0f, LastKey.Time);

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
