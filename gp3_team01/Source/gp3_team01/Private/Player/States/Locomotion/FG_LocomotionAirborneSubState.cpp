// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_DA_LedgevaultStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionLedgevaultSubState.h"
#include "Player/States/Locomotion/FG_LocomotionWallclimbSubState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/States/Locomotion/FG_LocomotionAirJumpSubState.h"
#include "Components/CapsuleComponent.h"
#include "Player/ABP_Player.h"

void UFG_LocomotionAirborneSubState::OnStateEnter_Implementation()
{
	Super::OnStateEnter_Implementation();
}

bool UFG_LocomotionAirborneSubState::OnStateTick_Implementation(float DeltaTime)
{
	Super::OnStateTick_Implementation(DeltaTime);
	//Input
	const UFG_DA_PlayerStats* Stats = FGPlayerCharacter->Stats;
	const UFG_DA_InputStats* InputStats = FGPlayerCharacter->Stats->InputStats;

	if (InputStats->GetWasGlidingJustPressed()) 
	{
		FGPlayerCharacter->StateMachine->Push(FGPlayerCharacter->GlidingCoreState);
		return true;
	}

	const bool HasNotReachedJumpLimit = Context->SubStateMachine->Count(Context->LocomotionAirJumpSubState) < 1;
	if (InputStats->GetWasJumpingJustPressed() && HasNotReachedJumpLimit) {
		Context->SubStateMachine->Push(Context->LocomotionAirJumpSubState);
		return true;
	}

	UFG_DA_MoveStats* AirborneStats = FGPlayerCharacter->Stats->AirborneStats;
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;

	const FVector Input = InputStats->GetCameraNormalisedInputVector();

	const FRichCurveKey LastKey = AirborneStats->AccelerationCurve->FloatCurve.GetLastKey();
	const float Timer = FMath::Clamp(GetTimeSinceEnter(), 0.0f, LastKey.Time);

	float Acceleration = AirborneStats->AccelerationCurve ? AirborneStats->AccelerationCurve->GetFloatValue(Timer) : 1.0f;
	Acceleration *= AirborneStats->MaxAcceleration;
	Locomotion->MoveRAW(Input, Acceleration, AirborneStats->MaxSpeed);
	Locomotion->ApplyFriction(AirborneStats);

	float RotationSpeed = AirborneStats->RotationCurve ? AirborneStats->RotationCurve->GetFloatValue(Timer) : 1.0f;
	RotationSpeed *= AirborneStats->MaxRotation;

	BroadcastAirMovingEvents(Input);
	//Enable this to add some soft rotation to the velocity's direction
	/*
	FVector GroundVel = FGPlayerCharacter->Collider->GetComponentVelocity();
	GroundVel.Z = 0;
	if (GroundVel.Size() > 5.f) //Magic Numbersss
	{
		FGPlayerCharacter->LocomotionComp->RotatePlayerRAW(FGPlayerCharacter->Collider->GetComponentVelocity().GetSafeNormal(), 1.f);
	}
	*/

	FVector ZInputDir = FGPlayerCharacter->Stats->InputStats->GetCameraNormalisedInputVector();
	FGPlayerCharacter->LocomotionComp->RotatePlayerRAW(ZInputDir, RotationSpeed);

	//you should pop yourself NOW - LTG
	
	//If inputting towards a ledge, ledgevault
	if (Locomotion->IsLedgeVaultableRAW(FGPlayerCharacter->MeshPivot->GetForwardVector(), Stats->LedgeStats->LedgeRayForward, Stats->LedgeStats->LedgeRayUp, Stats->LedgeStats->LedgeRayDown))
	{
		Locomotion->SaveLedgeGrabbablePosition(FGPlayerCharacter->MeshPivot->GetForwardVector(), FGPlayerCharacter->Stats->LedgeStats);

		FVector LedgeNormal0Z = Locomotion->LedgeNormal;
		LedgeNormal0Z.Z = 0;
		LedgeNormal0Z.Normalize();

		float LedgeInputDot = FVector::DotProduct(LedgeNormal0Z, Stats->InputStats->GetCameraNormalisedInputVector());
		if (LedgeInputDot <= 0 && Stats->InputStats->GetCameraNormalisedInputVector() != FVector::ZeroVector)
		{
			Locomotion->DisableLedgeGrab(0.5f);
			Context->SubStateMachine->Push(Context->LocomotionLedgevaultSubState);
			return true;
		}
	}

	//Handle push wallclimb
	if (FGPlayerCharacter->LocomotionComp->IsWallClimbableRAW(FGPlayerCharacter->MeshPivot->GetForwardVector(), Stats->LedgeStats->WallGrabRay, Stats->LedgeStats->WallInclineTreshold))
	{
		//Also has to input towards the wall to climb it
		float InputAndLedgeDot = FVector::DotProduct(Stats->InputStats->GetCameraNormalisedInputVector(), Locomotion->GetWallClimbNormal());
		if (InputAndLedgeDot < 0)
		{
			Context->SubStateMachine->Push(Context->LocomotionWallclimbSubState);
			return true;
		}
	}

	const bool IsGrounded = Context->IsGrounded();
	if (IsGrounded)
	{
		FGPlayerCharacter->GetABP()->Montage_Play(FGPlayerCharacter->LandMontage, 1, EMontagePlayReturnType::MontageLength, 0, true);
	}
	return !IsGrounded;
}

void UFG_LocomotionAirborneSubState::BroadcastAirMovingEvents(const FVector& Input)
{
	const bool IsMovingThisFrame = !Input.IsNearlyZero();

	if (bWasMovingPreviousFrame && IsMovingThisFrame)
	{
		OnStartAirMoving(FGPlayerCharacter);
	}
	else if (!bWasMovingPreviousFrame && !IsMovingThisFrame)
	{
		OnEndAirMoving(FGPlayerCharacter);
	}
	if (IsMovingThisFrame)
	{
		OnAirMoving(FGPlayerCharacter);
	}

	bWasMovingPreviousFrame = IsMovingThisFrame;
}
