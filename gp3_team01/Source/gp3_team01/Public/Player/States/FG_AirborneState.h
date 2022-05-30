// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/FG_PlayerState.h"
#include "FG_AirborneState.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP3_TEAM01_API UFG_AirborneState : public UFG_PlayerState
{
	GENERATED_BODY()
	
public:
	UFG_AirborneState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool OnStateTick_Implementation(float DeltaTime) final;
};
