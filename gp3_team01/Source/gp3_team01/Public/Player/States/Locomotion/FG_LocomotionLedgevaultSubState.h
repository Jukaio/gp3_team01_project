// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Locomotion/FG_LocomotionSubState.h"
#include "FG_LocomotionLedgevaultSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionLedgevaultSubState : public UFG_LocomotionSubState
{
	GENERATED_BODY()

	virtual void OnStatePush_Implementation() override;
	virtual void OnStatePop_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnStartLedgeVaulting(class AFG_PlayerCharacter* PlayerCharacter);

	// There shouldn't be a need for this one, since this is a fall-through state
	//UFUNCTION(BlueprintImplementableEvent)
	//void OnLedgeVaulting(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEndLedgeVaulting(class AFG_PlayerCharacter* PlayerCharacter);

	TArray<int> RandomIndeces;
};
