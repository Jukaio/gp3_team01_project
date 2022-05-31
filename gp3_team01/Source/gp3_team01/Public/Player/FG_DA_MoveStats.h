// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_MoveStats.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_DA_MoveStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Acceleration)
	float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Acceleration)
	class UCurveFloat* AccelerationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rotation)
	float MaxRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Rotation)
	class UCurveFloat* RotationCurve;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Friction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxSpeed;
};
