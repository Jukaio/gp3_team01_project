// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_LocomotionComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_TEAM01_API UFG_LocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	//Public functions
	UFG_LocomotionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// Adds force to the player with given input vector with said acceleration and disallows adding more than MaxSpeed with said speed
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Move(class UFG_DA_InputStats* InputStats, class UFG_DA_MoveStats* MoveStats);
	UFUNCTION(BlueprintCallable)
	void MoveRAW(FVector InputVector, float Acceleration, float MaxSpeed);

	/// <summary>
	/// Reduces velocity with friction
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void ApplyFriction(class UFG_DA_MoveStats* MoveStats);
	UFUNCTION(BlueprintCallable)
	void ApplyFrictionRAW(float Friction);

	/// <summary>
	/// Moves and applies friction
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void MoveWithFriction(class UFG_DA_InputStats* InputStats, class UFG_DA_MoveStats* MoveStats);
	
	/// <summary>
	/// Shoots a ray down and adds force to shape in order to hover Hoverdistance above the ground.
	/// Returns a bool if ray hit ground or not and sets bGrounded if it hits or not.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool Hover(class UFG_DA_HoverStats* HoverStats);
	UFUNCTION(BlueprintCallable)
	bool HoverRAW(float HoverDistance, float HoverStrength, float HoverDamping);

	/// <summary>
	/// Checks if ledge in the check direction is valid. The ledgecheck is in the shape of a hook
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool IsLedgeVaultableRAW(FVector CheckDirection, float RayLength, float RayLengthUp, float RayLengthDown);

	/// <summary>
	/// Makes an overlap check around given component.
	/// Returns a bool if anything collided with primitive component.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool CheckTouching(class UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable)
	void RotatePlayerRAW(FVector Direction, float RotationSpeed);

	/// <summary>
	/// Jumps. Also very momentarily disables floating
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void Jump(class UFG_DA_JumpStats* JumpStats);
	UFUNCTION(BlueprintCallable)
	void JumpRAW(float JumpForce, float RayDistance, float DisableFloatingTime);

	/// <summary>
	/// Disables floating for a period of time. 
	/// Use this if you want to say, jump from the ground because otherwise the Hover damping will subtract from the jump force and make jumping really inconsistent.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DisableFloat(float Duration = 0.5f);

	/// <summary>
	/// Adjusts input to be relative to camera. This function shouldnt be in this class probably
	/// </summary>
	UFUNCTION(BlueprintPure)
	FVector GetInputRelativeToCamera(const FVector InputVector, const USceneComponent* CameraTransform);

public: //Public variables
	UPROPERTY(BlueprintReadOnly)
	bool bGrounded;

private: //Private variables
	float DisabledFloatTimer = 0.f;

	UPROPERTY()
	class UPrimitiveComponent* Shape = nullptr;
	TSet<AActor*> LastCallResults;
	TSet<AActor*> CurrentCallResults;

};
