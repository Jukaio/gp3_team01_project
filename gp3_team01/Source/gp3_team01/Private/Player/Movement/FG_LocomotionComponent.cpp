// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "FG_BroadcastTemplate.h"
#include "Player/FG_DA_HoverStats.h"
#include "Player/FG_DA_JumpStats.h"
#include "Player/FG_DA_MoveStats.h"
#include "Player/FG_DA_InputStats.h"

// Sets default values for this component's properties
UFG_LocomotionComponent::UFG_LocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UFG_LocomotionComponent::BeginPlay()
{
	Super::BeginPlay();
	Shape = Cast<UPrimitiveComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
}


// Called every frame
void UFG_LocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DisabledFloatTimer = DisabledFloatTimer - DeltaTime;
}

void UFG_LocomotionComponent::Move(UFG_DA_InputStats* InputStats, UFG_DA_MoveStats* MoveStats)
{
	const FVector InputVector = InputStats->GetCameraNormalisedInputVector();
	MoveRAW(InputVector, MoveStats->MaxAcceleration, MoveStats->MaxSpeed);
}

void UFG_LocomotionComponent::MoveRAW(FVector InputVector, float Acceleration, float MaxSpeed)
{
	FVector AccelToAdd = InputVector * Acceleration;
	FVector GroundVelocity = GetOwner()->GetVelocity();

	GroundVelocity.Z = 0; //Makes sure that it only cares about horizontal velocity

	float AccelGroundDot = FVector::DotProduct(AccelToAdd, GroundVelocity.GetSafeNormal());
	if (AccelGroundDot <= 0)
	{
		//If below zero means it will not add velocity towards max speed, so reduce factor to 0
		AccelGroundDot = 0;
	}

	//I wish I had a better name for this fvector
	FVector DotFactoredAmount = AccelGroundDot * GroundVelocity.GetSafeNormal();

	if (GroundVelocity.Size() > MaxSpeed)
	{
		//When moving too fast, subtract the DotFactoredAmount to the AccelToAdd to prevent overshooting the maxspeed
		AccelToAdd = AccelToAdd - DotFactoredAmount;
	}

	Shape->AddForce(AccelToAdd, NAME_None, true);
}

void UFG_LocomotionComponent::ApplyFriction(UFG_DA_MoveStats* MoveStats)
{
	ApplyFrictionRAW(MoveStats->Friction);
}

void UFG_LocomotionComponent::ApplyFrictionRAW(float Friction)
{
	FVector frictionVector = GetOwner()->GetVelocity();
	frictionVector.Z = 0;
	frictionVector = frictionVector * Friction;

	Shape->AddForce(frictionVector * -1);
}

void UFG_LocomotionComponent::MoveWithFriction(UFG_DA_InputStats* InputStats, UFG_DA_MoveStats* MoveStats)
{
	Move(InputStats, MoveStats);
	ApplyFriction(MoveStats);
}

bool UFG_LocomotionComponent::Hover(UFG_DA_HoverStats* HoverStats)
{
	return HoverRAW(HoverStats->HoverDistance, HoverStats->HoverStrength, HoverStats->HoverDamping);
}

bool UFG_LocomotionComponent::HoverRAW(float HoverDistance, float HoverStrength, float HoverDamping)
{
	if (DisabledFloatTimer >= 0) //The float can be momentarily disabled from other actions
	{
		bGrounded = false;
		return false;
	}

	FVector RayDown = GetOwner()->GetActorLocation() - FVector(0, 0, HoverDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	FHitResult hit;
	if (GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), RayDown, ECC_WorldStatic, CollisionParams)) //Might need to change the ECC_WorldStatic to something different if we want moving platform interaction. Also needs velocity inheriting
	{
		if (!bGrounded)
		{
			GEngine->AddOnScreenDebugMessage(
				INDEX_NONE,
				0.5f,
				FColor::Magenta,
				FString::Printf(TEXT("Landed")));

			BroadcastResponse<IFG_LocomotionResponse>(hit, IFG_LocomotionResponse::Execute_OnLocomotionLand, this);
			//Put OnLand raise event here

		}
		float HitSubtractedHoverDistance = hit.Distance - HoverDistance; //Another shitty variable name, sorry
		float SpringForce = (HitSubtractedHoverDistance * HoverStrength) - (Shape->GetComponentVelocity().Z * HoverDamping);

		Shape->AddForce(FVector::DownVector * SpringForce, NAME_None, true);

		bGrounded = true;
		return true;
	}
	bGrounded = false;
	return false;
}

bool UFG_LocomotionComponent::IsLedgeVaultableRAW(FVector CheckDirection, float RayLength, float RayLengthUp, float RayLengthDown)
{
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	bool IsAboveClear;
	bool IsFrontClear;
	bool IsLedgeClear;
	bool IsClimbOntoClear;
	//If IsClimbOntoClear and IsLedgeClear are true and IsAboveClear and IsFrontClear are false, return true.

	FVector AboveRay = GetOwner()->GetActorLocation() + FVector::UpVector * RayLengthUp;

	//Checks if it's clear above me
	IsAboveClear = GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), AboveRay, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), AboveRay, FColor::Yellow, false, 1, 1, 5.f);

	//Checks forward if the ledge is valid
	IsLedgeClear = GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * RayLength, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * RayLength, FColor::Green, false, 1, 1, 5.f);
	//Save the ledge position and normal here!!

	//Checks if it's clear above and infront of me
	IsFrontClear = GetWorld()->LineTraceSingleByChannel(hit, AboveRay, AboveRay + CheckDirection * RayLength, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), AboveRay, AboveRay + CheckDirection * RayLength, FColor::Yellow, false, 1, 1, 5.f);

	//Checks if it's valid to climb ONTO said position.
	IsClimbOntoClear = GetWorld()->LineTraceSingleByChannel(hit, AboveRay + CheckDirection * RayLength, AboveRay + CheckDirection * RayLength + FVector::DownVector * RayLengthDown, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), AboveRay + CheckDirection * RayLength, AboveRay + CheckDirection * RayLength + FVector::DownVector * RayLengthDown, FColor::Green, false, 1, 1, 5.f);

	//All these rays should amount to a raycast that looks like a hook
	//I might do that the rays are in an array of 3 incase it's janky

	if (IsClimbOntoClear && IsLedgeClear && !IsAboveClear && !IsFrontClear)
	{
		GEngine->AddOnScreenDebugMessage(
			INDEX_NONE,
			0.f,
			FColor::Magenta,
			FString::Printf(TEXT("Can Vault"))); //Printf returns a string
		return true;
	}

	return false;
}

bool UFG_LocomotionComponent::CheckTouching(UPrimitiveComponent* Component)
{
	TArray<FOverlapResult> Results; 
	FComponentQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	
	const bool HasResults = GetWorld()->ComponentOverlapMultiByChannel
	(
		Results,
		Component,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorRotation(),
		ECC_WorldStatic, // Pass down later
		CollisionParams // Pass down later
	);

	CurrentCallResults.Reset();

	for (auto& Result : Results)
	{
		AActor* Actor = Result.GetActor();
		CurrentCallResults.Add(Result.GetActor());
	}

	for (auto& Element : CurrentCallResults)
	{
		if (!LastCallResults.Contains(Element)) {
			BroadcastResponse<IFG_LocomotionResponse>(Element, IFG_LocomotionResponse::Execute_OnLocomotionTouch, this, EFG_TouchingState::StartedTouching);
		}
	}

	for (auto& Result : Results)
	{
		AActor* Actor = Result.GetActor();
		BroadcastResponse<IFG_LocomotionResponse>(Actor, IFG_LocomotionResponse::Execute_OnLocomotionTouch, this, EFG_TouchingState::StillTouching);
	}

	for (auto& Element : LastCallResults)
	{
		if (!CurrentCallResults.Contains(Element)) {
			BroadcastResponse<IFG_LocomotionResponse>(Element, IFG_LocomotionResponse::Execute_OnLocomotionTouch, this, EFG_TouchingState::StoppedTouching);
		}
	}
	
	LastCallResults.Reset();
	LastCallResults = CurrentCallResults;
	return HasResults;
}

void UFG_LocomotionComponent::RotatePlayerRAW(FVector Direction, float RotationSpeed)
{
	if (Direction == FVector::ZeroVector)
	{
		return;
	}
	FQuat TargetRotation = Direction.ToOrientationQuat();
	FQuat PlayerRotation = FQuat::Slerp(Shape->GetForwardVector().ToOrientationQuat(), TargetRotation, RotationSpeed * GetWorld()->DeltaTimeSeconds);
	
	GetOwner()->SetActorRotation(PlayerRotation);
	

}

void UFG_LocomotionComponent::Jump(UFG_DA_JumpStats* JumpStats)
{
	JumpRAW(JumpStats->JumpForce, JumpStats->RayDistance, JumpStats->DisableFloatingTime);
}

void UFG_LocomotionComponent::JumpRAW(float JumpForce, float RayDistance, float DisableFloatingTime)
{
	DisableFloat(DisableFloatingTime); //Briefly disable floating because otherwise the damping hover force will make the jump inconsistently high.

	FVector RayDown = GetOwner()->GetActorLocation() - FVector(0, 0, RayDistance);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	FHitResult hit;
	if (GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), RayDown, ECC_WorldStatic, CollisionParams)) //Might need to change the ECC_WorldStatic to something different if we want moving platform interaction. Also needs velocity inheriting
	{
		BroadcastResponse<IFG_LocomotionResponse>(hit, IFG_LocomotionResponse::Execute_OnLocomotionJump, this);
	}
	FVector JumpVector = FVector::UpVector * JumpForce;
	JumpVector.X = GetOwner()->GetVelocity().X;
	JumpVector.Y = GetOwner()->GetVelocity().Y;

	Shape->SetPhysicsLinearVelocity(JumpVector);
}

void UFG_LocomotionComponent::DisableFloat(float Duration)
{
	DisabledFloatTimer = Duration;
}

FVector UFG_LocomotionComponent::GetInputRelativeToCamera(const FVector InputVector, const USceneComponent* CameraTransform)
{
	FVector NewVector = CameraTransform->GetRightVector() * InputVector.X + CameraTransform->GetForwardVector() * InputVector.Y;
	NewVector.Z = 0;
	NewVector.Normalize();
	return NewVector;
}

