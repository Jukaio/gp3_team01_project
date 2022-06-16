// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_MoveStats.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UFG_DA_MoveStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Acceleration)
	float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Acceleration)
	class UCurveFloat* AccelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	float MaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	class UCurveFloat* RotationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Friction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;
};
