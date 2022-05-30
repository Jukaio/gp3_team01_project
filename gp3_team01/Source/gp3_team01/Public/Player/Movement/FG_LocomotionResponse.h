// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FG_Touching.h"
#include "FG_LocomotionResponse.generated.h"

class UFG_LocomotionComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UFG_LocomotionResponse : public UInterface
{
	GENERATED_BODY()
};

/**
 * To call this use BroadcastTemplate and call: 
 * BroadcastResponse<IFG_LocomotionResponse>(other, IFG_LocomotionResponse::Execute_OnLand)
 */

class GP3_TEAM01_API IFG_LocomotionResponse
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// BP interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLocomotionJump(UFG_LocomotionComponent* LocomotionComponentOfCaller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLocomotionLand(UFG_LocomotionComponent* LocomotionComponentOfCaller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnLocomotionTouch(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State);
	
	// C++ interface
	virtual void OnLocomotionJump_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller);
	virtual void OnLocomotionLand_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller);
	virtual void OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State);
};
