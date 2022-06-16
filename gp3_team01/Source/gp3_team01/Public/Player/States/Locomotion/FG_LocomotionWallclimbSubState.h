// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "FG_LocomotionWallclimbSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionWallclimbSubState : public UFG_LocomotionSubState
{
	GENERATED_BODY()
	
	virtual void OnStatePush_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;
	virtual void OnStateExit_Implementation() override;
	virtual void OnStatePop_Implementation() override;

	bool bIsWallClimbingPossible = false;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnStarWallClimbing(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWallClimbing(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndWallClimbing(class AFG_PlayerCharacter* PlayerCharacter);
};
