// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_HoverStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UFG_DA_HoverStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverRayDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HoverDamping;

	
};
