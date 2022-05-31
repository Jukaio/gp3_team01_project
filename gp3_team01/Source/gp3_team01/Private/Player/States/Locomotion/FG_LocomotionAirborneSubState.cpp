// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"

void UFG_LocomotionAirborneSubState::OnStateEnter_Implementation()
{
	Timer = 0.0f;
}

bool UFG_LocomotionAirborneSubState::OnStateTick_Implementation(float DeltaTime)
{
	//Input
	const UFG_DA_InputStats* InputStats = FGPlayerCharacter->Stats->InputStats;
	const FVector Input = InputStats->GetCameraNormalisedInputVector();
	if (InputStats->GetWasGlidingJustPressed()) 
	{
		FGPlayerCharacter->StateMachine->Push(FGPlayerCharacter->GlidingCoreState);
		return true;
	}

	UFG_DA_MoveStats* AirborneStats = FGPlayerCharacter->Stats->AirborneStats;
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;

	float Acceleration = AirborneStats->AccelerationCurve ? AirborneStats->AccelerationCurve->GetFloatValue(Timer) : 1.0f;
	Acceleration *= AirborneStats->MaxAcceleration;
	Locomotion->MoveRAW(Input, Acceleration, AirborneStats->MaxSpeed);
	Locomotion->ApplyFriction(AirborneStats);

	float RotationSpeed = AirborneStats->RotationCurve ? AirborneStats->RotationCurve->GetFloatValue(Timer) : 1.0f;
	RotationSpeed *= AirborneStats->MaxRotation;

	FVector ZInputDir = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	FGPlayerCharacter->LocomotionComp->RotatePlayerRAW(ZInputDir, RotationSpeed);

	const FRichCurveKey LastKey = AirborneStats->AccelerationCurve->FloatCurve.GetLastKey();
	Timer += DeltaTime;
	Timer = FMath::Clamp(Timer, 0.0f, LastKey.Time);

	//you should pop yourself NOW - LTG
	const bool IsGrounded = FGPlayerCharacter->LocomotionComp->Hover(FGPlayerCharacter->Stats->HoverStats);
	return !IsGrounded;
}
