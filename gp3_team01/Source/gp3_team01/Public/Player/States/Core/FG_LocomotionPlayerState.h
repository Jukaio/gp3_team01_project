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

	virtual bool OnStateTick_Implementation(float DeltaTime) override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionAirJumpSubState* LocomotionAirJumpSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionLedgevaultSubState* LocomotionLedgevaultSubState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionWallclimbSubState* LocomotionWallclimbSubState;

	UFUNCTION(BlueprintPure)
	bool IsGroundedWithResult(TArray<FHitResult>& OutHit) const;

	UFUNCTION(BlueprintPure)
	bool IsGrounded() const;

	UFUNCTION(BlueprintPure)
	bool IsCoyoteGrounded() const;

	UFUNCTION(BlueprintPure)
	bool ShouldStartJump();

	void BufferJumpState();

	mutable TArray<FHitResult> GroundHit{ };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Instrument;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Bells;
	UPROPERTY()
	class UInstrumentChord* CurrentChord;
	UPROPERTY()
	class UFG_InstrumentComponent* InstrumentHandler;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerLand(class AFG_PlayerCharacter* PlayerCharacter);

private:
	void CheckAndSetGrounded(class UFG_LocomotionComponent* Locomotion, class UFG_DA_HoverStats* HoverStats);
	void PlayerLand();

	bool bIsGrounded = false;

	float CoyoteTimeStart = 0.0f;
	bool bWasGrounded = false;

	class UInstrumentChord* CurrentFallChord;

	// Wasting 3 bytes here :/ - A man's gotta do, what a man's gotta do.
	struct TimedInput
	{
		float TimePoint;
		bool State;
	};

	TArray<TimedInput> JumpBuffer;
};
