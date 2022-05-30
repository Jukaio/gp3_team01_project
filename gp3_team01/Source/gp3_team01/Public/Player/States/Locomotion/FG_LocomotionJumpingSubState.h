// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "FG_LocomotionJumpingSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionJumpingSubState : public UFG_LocomotionSubState
{
	GENERATED_BODY()
	
	virtual void OnStatePush_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;
};
