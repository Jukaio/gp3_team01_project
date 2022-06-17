// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_LedgevaultStats.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_DA_LedgevaultStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LedgeRayForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LedgeRayUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LedgeRayDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LedgeInclineTreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallHugForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallHorizontalClimbForce = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallClimbForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallClimbMaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallGrabRay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WallClimb)
	float WallInclineTreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LedgevaultJumpForce = 2000.f;
	
};
