// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

UENUM(BlueprintType)
enum EFG_TouchingState
{
	StartedTouching UMETA(DisplayName = "Started Touching"),
	StillTouching UMETA(DisplayName = "Still Touching"),
	StoppedTouching UMETA(DisplayName = "Stopped Touching")
};
