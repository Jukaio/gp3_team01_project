// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/States/FG_PlayerState.h"
#include "FG_LocomotionPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_LocomotionPlayerState : public UFG_PlayerState
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFG_LocomotionPlayerState();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool OnStateTick_Implementation(float DeltaTime) final;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_SFSM* SubStateMachine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionIdleSubState* LocomotionIdleSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionMovingSubState* LocomotionMovingSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionJumpingSubState* LocomotionJumpingSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionAirborneSubState* LocomotionAirborneSubState;
};
