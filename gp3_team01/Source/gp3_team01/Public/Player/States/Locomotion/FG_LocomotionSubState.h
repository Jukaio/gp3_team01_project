// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/FG_PlayerState.h"
#include "FG_LocomotionSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionSubState : public UFG_PlayerState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	class UFG_LocomotionPlayerState* Context;

	UFUNCTION(BlueprintPure)
	class UFG_LocomotionPlayerState* GetContext() const;
};
