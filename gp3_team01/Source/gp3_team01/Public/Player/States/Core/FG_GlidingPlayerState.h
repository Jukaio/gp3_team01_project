// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/States/FG_PlayerState.h"
#include "FG_GlidingPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_GlidingPlayerState : public UFG_PlayerState
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFG_GlidingPlayerState();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_SFSM* SubStateMachine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_GlidingIdleSubState* GlidingIdleSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_GlidingSteeringSubState* GlidingSteeringSubState;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UFG_GliderComponent* GliderComponent;
	bool bIsColliding;

	virtual void OnStateEnter_Implementation() final;
	virtual void OnStateExit_Implementation() final;
	virtual bool OnStateTick_Implementation(float DeltaTime) final;
	UFUNCTION()
	void OnCollision(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	                 FVector Vector, const FHitResult& HitResult);
};
