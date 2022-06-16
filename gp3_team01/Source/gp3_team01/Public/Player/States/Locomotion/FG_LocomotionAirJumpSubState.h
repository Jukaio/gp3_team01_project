// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "FG_LocomotionAirJumpSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionAirJumpSubState : public UFG_LocomotionSubState
{
	GENERATED_BODY()

	void OnStatePush_Implementation() override;
	void OnStatePop_Implementation() override;
	bool OnStateTick_Implementation(float DeltaTime) override;

	bool bIsContinousJumpingPossible = false;

public:

	UFUNCTION(BlueprintImplementableEvent)
	void OnStartAirJumping(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnContinousAirJumping(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndAirJumping(class AFG_PlayerCharacter* PlayerCharacter);
};
