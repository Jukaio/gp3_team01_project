// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "FG_BroadcastTemplate.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"

void UFG_LocomotionMovingSubState::OnStateEnter_Implementation()
{
	Super::OnStateEnter_Implementation();
	OnStartMoving(FGPlayerCharacter);
}

void UFG_LocomotionMovingSubState::OnStatePush_Implementation()
{
	Super::OnStatePush_Implementation();

}

void UFG_LocomotionMovingSubState::OnStateExit_Implementation()
{
	OnEndMoving(FGPlayerCharacter);
}

bool UFG_LocomotionMovingSubState::OnStateTick_Implementation(float DeltaTime)
{
	Super::OnStateTick_Implementation(DeltaTime);

	const FVector Input = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	
	if (Context->ShouldStartJump())
	{
		Context->SubStateMachine->Push(Context->LocomotionJumpingSubState);
		return true;
	}

	const float WorldTime = GetWorld()->TimeSeconds;
	const float Difference = WorldTime - LastTimeMoveWasPlayed;
	if (Context->InstrumentHandler != nullptr && Difference > 8.0f)
	{
		const int RandomIndex = FMath::Rand() % Context->Bells->Num();
		Context->InstrumentHandler->PlayChord(Context->Bells->Find(RandomIndex), 0.50f, 1.0f, 2, 0.25f, 1.0f);
		LastTimeMoveWasPlayed = WorldTime;
	}


	UFG_DA_MoveStats* MoveStats = FGPlayerCharacter->Stats->MoveStats;
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;

	const FRichCurveKey LastKey = MoveStats->AccelerationCurve->FloatCurve.GetLastKey();
	const float Timer = FMath::Clamp(GetTimeSinceEnter(), 0.0f, LastKey.Time);

	float Acceleration = MoveStats->AccelerationCurve ? MoveStats->AccelerationCurve->GetFloatValue(Timer) : 1.0f;
	Acceleration *= MoveStats->MaxAcceleration;
	Locomotion->MoveRAW(Input, Acceleration, MoveStats->MaxSpeed);
	Locomotion->ApplyFriction(MoveStats);
	
	float RotationSpeed = MoveStats->RotationCurve ? MoveStats->RotationCurve->GetFloatValue(Timer) : 1.0f;
	RotationSpeed *= MoveStats->MaxRotation;

	FVector ZInputDir = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	FGPlayerCharacter->LocomotionComp->RotatePlayerRAW(ZInputDir, RotationSpeed);

	//If hover ray doesnt hit, go airborne!
	if (!Context->IsCoyoteGrounded())
	{
		Context->SubStateMachine->Push(Context->LocomotionAirborneSubState);
		return true;
	}

	if (!Input.IsNearlyZero()) 
	{
		OnMoving(FGPlayerCharacter);
		for (auto& Hit : Context->GroundHit)
		{
			BroadcastResponse<IFG_LocomotionResponse>(Hit, IFG_LocomotionResponse::Execute_OnLocomotionMove, Locomotion);
		}
		return true;
	}

	return false;
}

