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
	UPROPERTY()
	UFG_LocomotionComponent* Locomotion;
	UPROPERTY()
	UPrimitiveComponent* Primitive;

	TArray<int> RandomIndeces;
	float LastTimeMoveWasPlayed = 0.0f;
	float PlayTimeBuffer = 0.0f;
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

	virtual void OnStatePush_Implementation() override;
	virtual void OnStatePop_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Instrument;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Chimes;
	UPROPERTY()
	class UInstrumentChord* CurrentChord;
	UPROPERTY()
	class UFG_InstrumentComponent* InstrumentHandler;

	UFUNCTION()
	void OnCollision(UPrimitiveComponent* PrimitiveComponent, AActor* Actor, UPrimitiveComponent* PrimitiveComponent1,
	                 FVector Vector, const FHitResult& HitResult);
	
};
