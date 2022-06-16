// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_HoverStats.h"
#include "FG_DA_JumpStats.h"
#include "FG_DA_MoveStats.h"
#include "FG_DA_InputStats.h"
#include "FG_DA_PlayerStateClasses.h"
#include "FG_DA_PlayerStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UFG_DA_PlayerStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_PlayerStateClasses* PlayerStateClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_InputStats* InputStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_MoveStats* MoveStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_MoveStats* AirborneStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_HoverStats* HoverStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_JumpStats* JumpStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_JumpStats* AirJumpStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_LedgevaultStats* LedgeStats;
};
