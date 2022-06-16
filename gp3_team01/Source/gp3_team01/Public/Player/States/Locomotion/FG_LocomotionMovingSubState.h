// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "FG_LocomotionMovingSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionMovingSubState : public UFG_LocomotionSubState
{
	GENERATED_BODY()
	
	virtual void OnStateEnter_Implementation() override;
	virtual void OnStatePush_Implementation() override;
	virtual void OnStateExit_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;

	float LastTimeMoveWasPlayed = 0.0f;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartMoving(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMoving(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndMoving(class AFG_PlayerCharacter* PlayerCharacter);
};
