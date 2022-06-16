// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Glider.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UABP_Glider : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsActive;

	UPROPERTY(BlueprintReadWrite)
	float VelocityDist;
};
