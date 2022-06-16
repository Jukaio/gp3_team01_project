// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_FG_IntegerData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GP3_TEAM01_API UDA_FG_IntegerData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Reset();

	UFUNCTION(BlueprintPure)
	int32 Get() const;

	UFUNCTION(BlueprintCallable)
	void Set(int32 Value);

	UFUNCTION(BlueprintCallable)
	void Increment(int32 Value);

	UFUNCTION(BlueprintCallable)
	void Decrement(int32 Value);

private:
	int32 Data = 0;
};
