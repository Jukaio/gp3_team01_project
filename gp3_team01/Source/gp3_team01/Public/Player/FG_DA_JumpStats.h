// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_JumpStats.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_DA_JumpStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RayDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DisableFloatingTime;
};
