// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_CameraStats.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_DA_CameraStats : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CameraFollowSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CameraPanSpeed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaxTurnAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinimumTurnAngle;
	
};
