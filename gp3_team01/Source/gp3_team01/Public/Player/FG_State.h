// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FG_State.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UFG_State : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3_TEAM01_API IFG_State
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// BP interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStateEnter();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStatePush();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool OnStateTick(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStatePop();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnStateExit();

	// C++ interface
	virtual void OnStateEnter_Implementation();
	virtual void OnStatePush_Implementation();
	virtual bool OnStateTick_Implementation(float DeltaTime);
	virtual void OnStatePop_Implementation();
	virtual void OnStateExit_Implementation();
};
