// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Locomotion/FG_LocomotionWallclimbSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_DA_LedgevaultStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/ABP_Player.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Locomotion/FG_LocomotionLedgevaultSubState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirJumpSubState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"

void UFG_LocomotionWallclimbSubState::OnStatePush_Implementation()
{
	Super::OnStatePush_Implementation();
	OnStarWallClimbing(FGPlayerCharacter);
	bIsWallClimbingPossible = true;
	FGPlayerCharacter->GetABP()->bIsClimbing = true;

	const int RandomIndex = FMath::Rand() % Context->Bells->Num();
	//Context->InstrumentHandler->PlayChord(Context->Bells->Find(RandomIndex), 0.50f, 1.5f, 0);
}

bool UFG_LocomotionWallclimbSubState::OnStateTick_Implementation(float DeltaTime)
{
	Super::OnStateTick_Implementation(DeltaTime);

	UFG_DA_PlayerStats* Stats = FGPlayerCharacter->Stats;
	UFG_LocomotionComponent* LocoComp = FGPlayerCharacter->LocomotionComp;
	UFG_DA_InputStats* Input = FGPlayerCharacter->Stats->InputStats;

	const bool HasNotReachedJumpLimit = Context->SubStateMachine->Count(Context->LocomotionAirJumpSubState) < 1;
	if (Stats->InputStats->GetWasJumpingJustPressed() && HasNotReachedJumpLimit) {
		Context->SubStateMachine->Push(Context->LocomotionAirJumpSubState);
		return true;
	}

	//Is wall still climbable? If it isnt, pop
	if (!LocoComp->IsWallClimbableRAW(FGPlayerCharacter->MeshPivot->GetForwardVector(), Stats->LedgeStats->WallGrabRay, Stats->LedgeStats->WallInclineTreshold))
	{
		return false;
	}

	//If expended wallclimb duration, pop
	if (LocoComp->WallClimbDuration <= 0)
	{
		return false;
	}

	//Is there a valid ledge as I am inputting towards the wall? If there is, push ledgevault
	if (LocoComp->IsLedgeVaultableRAW(FGPlayerCharacter->MeshPivot->GetForwardVector(), Stats->LedgeStats->LedgeRayForward, Stats->LedgeStats->LedgeRayUp, Stats->LedgeStats->LedgeRayDown))
	{

		LocoComp->SaveLedgeGrabbablePosition(FGPlayerCharacter->MeshPivot->GetForwardVector(), FGPlayerCharacter->Stats->LedgeStats);
		FVector LedgeNormal0Z = LocoComp->LedgeNormal;
		LedgeNormal0Z.Z = 0;
		LedgeNormal0Z.Normalize();
		float LedgeInputDot = FVector::DotProduct(LedgeNormal0Z, Stats->InputStats->GetCameraNormalisedInputVector());
		if (LedgeInputDot <= 0 && Stats->InputStats->GetCameraNormalisedInputVector() != FVector::ZeroVector)
		{
			LocoComp->DisableLedgeGrab(0.5f);
			Context->SubStateMachine->Push(Context->LocomotionLedgevaultSubState);
			return true;
		}
		
		
	}

	LocoComp->WallClimbDuration -= DeltaTime;
	
	LocoComp->HandleClimbOnWallRAW(Stats->LedgeStats->WallHugForce, Stats->LedgeStats->WallClimbForce, Stats->LedgeStats->WallClimbMaxSpeed);
	FGPlayerCharacter->Collider->AddForce(Input->GetCameraNormalisedInputVector() * Stats->LedgeStats->WallHorizontalClimbForce, NAME_None, true);


	//If not inputting or inputting a direction away from wall, pop
	float InputAndLedgeDot = FVector::DotProduct(Stats->InputStats->GetCameraNormalisedInputVector(), LocoComp->GetWallClimbNormal());
	
	OnWallClimbing(FGPlayerCharacter);

	if (InputAndLedgeDot > 0 || Stats->InputStats->GetCameraNormalisedInputVector() == FVector::ZeroVector)
	{
		return false;
	}

	return true;
}

void UFG_LocomotionWallclimbSubState::OnStateExit_Implementation()
{
	OnStatePop_Implementation();
}

void UFG_LocomotionWallclimbSubState::OnStatePop_Implementation()
{
	bIsWallClimbingPossible = false;
	FGPlayerCharacter->LocomotionComp->DisableWallClimb();
	FGPlayerCharacter->LocomotionComp->DisableLedgeGrab();
	FGPlayerCharacter->GetABP()->bIsClimbing = false;
	OnEndWallClimbing(FGPlayerCharacter);
}
