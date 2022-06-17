// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Player.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UABP_Player : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float VelocityDist;

	UPROPERTY(BlueprintReadWrite)
	float VerticalVelocity;

	UPROPERTY(BlueprintReadWrite)
	bool bIsGrounded;

	UPROPERTY(BlueprintReadWrite)
	bool bIsGliding;

	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbing;
};
