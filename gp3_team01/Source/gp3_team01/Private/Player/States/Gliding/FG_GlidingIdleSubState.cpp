// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Gliding/FG_GlidingIdleSubState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"

bool UFG_GlidingIdleSubState::OnStateTick_Implementation(float DeltaTime)
{
	UFG_GliderComponent* Glider = FGPlayerCharacter->Glider;
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
	
	Glider->Glide();
	const bool IsGlidingPressed = FGPlayerCharacter->Stats->InputStats->GetIsGlidingPressed();
	//const bool IsGrounded = Locomotion->Hover(FGPlayerCharacter->Stats->HoverStats);
	return IsGlidingPressed;  // Return false to pop this state
}
