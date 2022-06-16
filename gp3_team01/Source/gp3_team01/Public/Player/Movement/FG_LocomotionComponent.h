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
	bool Hover(class UFG_DA_HoverStats* HoverStats, TArray<FHitResult>& hit);
	UFUNCTION(BlueprintCallable)
	bool HoverRAW(float HoverDistance, float HoverRay, float HoverStrength, float HoverDamping, TArray<FHitResult>& Hits);

	/// <summary>
	/// Applies gravity. Yep.
	/// </summary>
	void ApplyGravity(float Gravity);

	/// <summary>
	/// Checks if ledge in the check direction is valid. The ledgecheck is in the shape of a hook. Also saves the position for ledgehang and normal
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool IsLedgeVaultableRAW(FVector CheckDirection, float RayLength, float RayLengthUp, float RayLengthDown);

	/// <summary>
	/// Checks if wall in given direction is valid to climb on. WallLeanTreshold is a value between 1-0 how sharp the wall incline is allowed to be
	/// </summary>
	UFUNCTION(BlueprintCallable)
	bool IsWallClimbableRAW(FVector CheckDirection, float RayLength, float WallLeanTreshold);

	/// <summary>
	/// Saves the ledge normal and position which is used when Hanging on the ledge.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void SaveLedgeGrabbablePosition(FVector CheckDirection, class UFG_DA_LedgevaultStats* LedgeStats);

	/// <summary>
	/// Hangs onto the ledge at the position which the IsLedgeVaultableRaw saved (Basically teleports and resets velocity onto the ledge
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void HangOnLedgeRAW();

	/// <summary>
	/// Handles Climbing up walls with the normal given from HangOnLedge
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void HandleClimbOnWallRAW(float WallHugForce, float WallClimbForce, float WallClimbMaxSpeed);

	/// <summary>
	/// Disables the ability to grab ledges. Is called when you leave the Ledgevault state so you dont regrab the ledge instantly after
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DisableLedgeGrab(float Duration = 0.5f);

	/// <summary>
	/// Disables the ability to latch onto walls for a duration.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DisableWallClimb(float Duration = 0.5f);

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

	void JumpRAW(float JumpForce, float RayDistance);

	/// <summary>
	/// Disables floating for a period of time. 
	/// Use this if you want to say, jump from the ground because otherwise the Hover damping will subtract from the jump force and make jumping really inconsistent.
	/// </summary>
	UFUNCTION(BlueprintCallable)
	void DisableFloat(float Duration = 0.25f);

	UFUNCTION(BlueprintCallable)
	bool HasJustJumped() const;

	/// <summary>
	/// Adjusts input to be relative to camera. This function shouldnt be in this class probabl
	/// NOW OBSELETE, USE THE ONE FROM INPUTSTATS INSTEAD
	/// </summary>
	UFUNCTION(BlueprintPure)
	FVector GetInputRelativeToCamera(const FVector InputVector, const USceneComponent* CameraTransform);

	/// <summary>
	/// Get the normal of the wall you last climbed on/is climbing on
	/// </summary>
	UFUNCTION(BlueprintPure)
	FVector GetWallClimbNormal();

public: //Public variables


	//Set this to MovingPlatforms
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> PlatformCollisionChannelEnum;

	//This is what the player mesh will be parented to
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USceneComponent* VisualShape;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //How long can you climb. This should really be in the data asset buuut I'm lazy
	float WallClimbLength = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroundInclineTreshold = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LedgeInclineTreshold = 0.5f;

	UPROPERTY(BlueprintReadOnly)
	bool bGrounded;

	float WallClimbDuration = 0.f;

	FVector LedgePosition; //The position where you'll ledgehang
	FVector LedgeNormal; //The normal of the ledge you're hanging on

private: //Private variables
	float DisabledFloatTimer = 0.f;
	float DisabledLedgeGrabTimer = 0.f; //better than smash bros melee planking
	float DisabledWallClimbTimer = 0.f;

	

	UPROPERTY()
	class UPrimitiveComponent* Shape = nullptr;

	

	TSet<AActor*> LastCallResults;
	TSet<AActor*> CurrentCallResults;

	

	FVector WallClimbNormal; //The normal of the wall you're trying to climb on.



};
