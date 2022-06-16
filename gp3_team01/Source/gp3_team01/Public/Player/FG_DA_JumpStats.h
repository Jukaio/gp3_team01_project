// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_JumpStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UFG_DA_JumpStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Force)
	float JumpForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Force)
	float JumpReleaseCutoffAmount = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Force)
	class UCurveFloat* ForceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RayDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DisableFloatingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Gravity = -980.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoyoteTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpBufferThreshold = 0.5f;
};
