// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ABP_Yak.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UABP_Yak : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	float VelocityDist;

	UPROPERTY(BlueprintReadWrite)
	class UAnimMontage* BounceMontage;
	
};
