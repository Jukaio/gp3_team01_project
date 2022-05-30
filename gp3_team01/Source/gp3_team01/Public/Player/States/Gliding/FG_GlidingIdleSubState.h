// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/Gliding/FG_GlidingSubState.h"
#include "FG_GlidingIdleSubState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_GlidingIdleSubState : public UFG_GlidingSubState
{
	GENERATED_BODY()
	
public:
	virtual bool OnStateTick_Implementation(float DeltaTime);
};
