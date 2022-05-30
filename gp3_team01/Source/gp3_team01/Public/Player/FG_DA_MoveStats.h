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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundMaxSpeed;
};
