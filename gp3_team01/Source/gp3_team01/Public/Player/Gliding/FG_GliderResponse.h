// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FG_Touching.h"
#include "FG_GliderResponse.generated.h"

class UFG_GliderComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UFG_GliderResponse : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GP3_TEAM01_API IFG_GliderResponse
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGliderLand(UFG_GliderComponent* GliderComponentOfCaller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnGliderTouch(UFG_GliderComponent* GliderComponentOfCaller, EFG_TouchingState State);

	virtual void OnGliderLand_Implementation(UFG_GliderComponent* GliderComponentOfCaller);
	virtual void OnGliderTouch_Implementation(UFG_GliderComponent* GliderComponentOfCaller, EFG_TouchingState State);

};
