// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_PlayerStateClasses.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UFG_DA_PlayerStateClasses : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Core)
	TSubclassOf<class UFG_LocomotionPlayerState> LocomotionPlayerState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Core)
	TSubclassOf<class UFG_GlidingPlayerState> GlidingPlayerState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionIdleSubState> LocomotionIdleSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionJumpingSubState> LocomotionJumpSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionAirborneSubState> LocomotionAirborneSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionAirJumpSubState> LocomotionAirJumpSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionMovingSubState> LocomotionMovingSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionLedgevaultSubState> LocomotionLedgevaultSubStateClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Locomotion)
	TSubclassOf<class UFG_LocomotionWallclimbSubState> LocomotionWallclimbSubStateClass;
};
