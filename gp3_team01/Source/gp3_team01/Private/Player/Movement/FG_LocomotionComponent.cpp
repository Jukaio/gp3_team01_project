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
#include "Player/FG_DA_LedgevaultStats.h"
#include "Player/FG_DA_InputStats.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LevelElements/MovingPlatform/FG_MovingPlatform.h"

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
	VisualShape = Cast<AFG_PlayerCharacter>(GetOwner())->MeshPivot;
}


// Called every frame
void UFG_LocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DisabledFloatTimer = DisabledFloatTimer - DeltaTime;
	DisabledLedgeGrabTimer = DisabledLedgeGrabTimer - DeltaTime;
	DisabledWallClimbTimer = DisabledWallClimbTimer - DeltaTime;
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

bool UFG_LocomotionComponent::Hover(UFG_DA_HoverStats* HoverStats, TArray<FHitResult>& Hits)
{
	return HoverRAW(HoverStats->HoverDistance, HoverStats->HoverRayDistance, HoverStats->HoverStrength, HoverStats->HoverDamping, Hits);
}

bool UFG_LocomotionComponent::HoverRAW(float HoverDistance, float HoverRay, float HoverStrength, float HoverDamping, TArray<FHitResult>& Hits)
{
	if (DisabledFloatTimer >= 0) //The float can be momentarily disabled from other actions
	{
		bGrounded = false;
		return false;
	}

	FVector RayDown = GetOwner()->GetActorLocation() - FVector(0, 0, HoverRay);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(UEngineTypes::ConvertToObjectType(PlatformCollisionChannelEnum));
	CollisionParams.bIgnoreTouches = true;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoredActors;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(PlatformCollisionChannelEnum));


	//if (GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), RayDown, ECC_WorldStatic, CollisionParams)) //Might need to change the ECC_WorldStatic to something different if we want moving platform interaction. Also needs velocity inheriting
	//A tiny little Magic number here, being the width of this ray
	TArray<FHitResult> CurrentHitResults;
	bool PreviousGrounded = bGrounded;

	if (GetWorld()->LineTraceMultiByChannel(CurrentHitResults, Shape->GetComponentLocation(), RayDown - FVector::DownVector * 20.f, ECC_WorldStatic, CollisionParams))
	{
		bGrounded = true;
	}
	else if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Shape->GetComponentLocation(), RayDown, 20.f, ObjectTypesArray, true, IgnoredActors, EDrawDebugTrace::ForOneFrame, CurrentHitResults, true))
	{
		bGrounded = true;
	}
	else
	{
		bGrounded = false;
	}

	
	if (bGrounded == true)
	{
		for (auto& NewHit : CurrentHitResults)
		{
			if (FVector::DotProduct(FVector::UpVector, NewHit.Normal) < GroundInclineTreshold)
			{
				return false;
			}

			if (!PreviousGrounded)
			{
				WallClimbDuration = WallClimbLength;
				GEngine->AddOnScreenDebugMessage(
					INDEX_NONE,
					0.5f,
					FColor::Magenta,
					FString::Printf(TEXT("Landed")));

				BroadcastResponse<IFG_LocomotionResponse>(NewHit, IFG_LocomotionResponse::Execute_OnLocomotionLand, this);
				//Put OnLand raise event here

			}
			float HitSubtractedHoverDistance = NewHit.Distance - HoverDistance; //Another shitty variable name, sorry
			float SpringForce = (HitSubtractedHoverDistance * HoverStrength) - (Shape->GetComponentVelocity().Z * HoverDamping);

			Shape->AddForce(FVector::DownVector * SpringForce, NAME_None, true);

			if (NewHit.GetActor() && NewHit.GetActor()->ActorHasTag(FName(TEXT("MovingPlatform"))))
			{
				AFG_MovingPlatform* Plat = Cast<AFG_MovingPlatform>(NewHit.GetActor()); //wow cast in tick bro, it will ruin the project!!!
				if (Plat)
				{
				
					GEngine->AddOnScreenDebugMessage(
						INDEX_NONE,
						0.0f,
						FColor::Magenta,
						FString::Printf(TEXT("Moving with a platform")));
					GetOwner()->AddActorWorldOffset(Plat->DeltaMovementVector, true);
				}
			
			
			}
		}
	}


	for (auto& Hit : Hits)
	{
		if (!CurrentHitResults.ContainsByPredicate([&Hit](const FHitResult& Other){ return Other.Actor != Hit.Actor; }))
		{
			BroadcastResponse<IFG_LocomotionResponse>(Hit, IFG_LocomotionResponse::Execute_OnLocomotionLeft, this);
		}
	}

	//TSet<FHitResult> CurrentHits;
	//TSet<FHitResult> IntersectingHits = CurrentHits.Intersect(PreviousHits);
	//TSet<FHitResult> DifferenceHits = CurrentHits.Difference(PreviousHits);
	
	//for (auto& OldHit : DifferenceHits)
	//{
	//}

	//for (auto& NewHit : HitResults)
	//{
	//	if (Hits.Contains(NewHit))
	//	{

	//	}

	//	if (NewHit.Actor != Hit.Actor)
	//	{
	//		if (Hit.Actor != nullptr) {
	//			BroadcastResponse<IFG_LocomotionResponse>(Hit, IFG_LocomotionResponse::Execute_OnLocomotionLeft, this);
	//		}
	//	}
	//}
	Hits = CurrentHitResults;

	return bGrounded;
}

void UFG_LocomotionComponent::ApplyGravity(float Gravity)
{
	Shape->AddForce(FVector::UpVector * Gravity, NAME_None, true);
}


bool UFG_LocomotionComponent::IsLedgeVaultableRAW(FVector CheckDirection, float RayLength, float RayLengthUp, float RayLengthDown)
{
	if (DisabledLedgeGrabTimer >= 0)
	{
		return false;
	}
	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bIgnoreTouches = true;

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
	//IsLedgeClear = GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * RayLength, ECC_WorldStatic, CollisionParams);
	//DrawDebugLine(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * RayLength, FColor::Green, false, 1, 1, 5.f);
	//This is dead code for now as it is redundant. Get rid of the ForwardRay parameter in the future
	IsLedgeClear = true;

	//Checks if it's clear above and infront of me
	IsFrontClear = GetWorld()->LineTraceSingleByChannel(hit, AboveRay, AboveRay + CheckDirection * RayLength, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), AboveRay, AboveRay + CheckDirection * RayLength, FColor::Yellow, false, 1, 1, 5.f);

	//Checks if it's valid to climb ONTO said position.
	IsClimbOntoClear = GetWorld()->LineTraceSingleByChannel(hit, AboveRay + CheckDirection * RayLength, AboveRay + CheckDirection * RayLength + FVector::DownVector * RayLengthDown, ECC_WorldStatic, CollisionParams);
	DrawDebugLine(GetWorld(), AboveRay + CheckDirection * RayLength, AboveRay + CheckDirection * RayLength + FVector::DownVector * RayLengthDown, FColor::Green, false, 1, 1, 5.f);
	if (FVector::DotProduct(FVector::UpVector, hit.ImpactNormal) < LedgeInclineTreshold)
	{
		//If the surface youre trying to climb onto is too slanted, the ledge isnt valid
		IsClimbOntoClear = false;
	}

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

bool UFG_LocomotionComponent::IsWallClimbableRAW(FVector CheckDirection, float RayLength, float WallLeanTreshold)
{
	if (DisabledWallClimbTimer >= 0)
	{
		return false;
	}

	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bIgnoreTouches = true;
	

	bool DidRayHit = GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * RayLength, ECC_WorldStatic, CollisionParams);
	FVector WallNormal0Z = hit.Normal;
	WallNormal0Z.Z = 0;
	WallNormal0Z.Normalize();

	if (FVector::DotProduct(hit.Normal, WallNormal0Z) >  WallLeanTreshold)
	{
		
		WallClimbNormal = WallNormal0Z;
		return true;
	}

	return false;
}

void UFG_LocomotionComponent::HandleClimbOnWallRAW(float WallHugForce, float WallClimbForce, float WallClimbMaxSpeed)
{
	if (DisabledWallClimbTimer >= 0)
	{
		return;
	}
	FVector WallHugVector = (WallClimbNormal * WallHugForce) * -1;
	FVector WallClimbVector = FVector::UpVector * WallClimbForce;

	Shape->AddForce(WallHugVector, NAME_None, true);

	FVector WallNormal0Z = WallClimbNormal * -1;
	WallNormal0Z.Z = 0;
	WallNormal0Z.Normalize();
	RotatePlayerRAW(WallNormal0Z, 10.f);

	ApplyFrictionRAW(250.f); //MAGIC NUMBEEEER

	
	if (Shape->GetComponentVelocity().Z < WallClimbMaxSpeed)
	{
		Shape->AddForce(WallClimbVector, NAME_None, true);
	}

}

void UFG_LocomotionComponent::SaveLedgeGrabbablePosition(FVector CheckDirection, UFG_DA_LedgevaultStats* LedgeStats)
{
	//This method is unused for now

	if (DisabledLedgeGrabTimer >= 0)
	{
		return;
	}
	FVector OwnerStart = GetOwner()->GetActorLocation();

	FHitResult hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bIgnoreTouches = true;

	GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + CheckDirection * LedgeStats->LedgeRayForward, ECC_WorldStatic, CollisionParams);
	LedgePosition = hit.Location;
	LedgeNormal = hit.Normal;
	bool Bruh = GetWorld()->LineTraceSingleByChannel(
		hit, 
		OwnerStart + CheckDirection * LedgeStats->LedgeRayForward + FVector::UpVector * LedgeStats->LedgeRayUp,
		OwnerStart + CheckDirection * LedgeStats->LedgeRayForward + FVector::UpVector * LedgeStats->LedgeRayUp + FVector::DownVector * LedgeStats->LedgeRayDown,
		ECC_WorldStatic,
		CollisionParams
		);
	LedgePosition.Z = hit.ImpactPoint.Z;

	
}

void UFG_LocomotionComponent::HangOnLedgeRAW()
{
	FVector TeleportPosition = LedgePosition + LedgeNormal * 30.f;
	GetOwner()->SetActorLocation(TeleportPosition);
	FVector LedgeNormal0Z = LedgeNormal;
	LedgeNormal0Z.Z = 0;
	LedgeNormal0Z.Normalize();

	RotatePlayerRAW(LedgeNormal * -1.f, 25.f);

	Shape->SetPhysicsLinearVelocity(FVector::ZeroVector);
	
}



void UFG_LocomotionComponent::DisableLedgeGrab(float Duration)
{
	DisabledLedgeGrabTimer = Duration;
}

void UFG_LocomotionComponent::DisableWallClimb(float Duration)
{
	DisabledWallClimbTimer = Duration;
}

bool UFG_LocomotionComponent::CheckTouching(UPrimitiveComponent* Component)
{
	TArray<FOverlapResult> Results; 
	FComponentQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bIgnoreBlocks = true;
	const bool HasResults = GetWorld()->ComponentOverlapMultiByChannel
	(
		Results,
		Component,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorRotation(),
		ECC_WorldStatic, // Pass down later
		CollisionParams // Pass down later
	);



	//if (GetWorld()->LineTraceSingleByChannel(hit, GetOwner()->GetActorLocation(), RayDown, ECC_WorldStatic, CollisionParams)) //Might need to change the ECC_WorldStatic to something different if we want moving platform interaction. Also needs velocity inheriting
	//A tiny little Magic number here, being the width of this ray

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
	if (Direction == FVector::ZeroVector || VisualShape == nullptr)
	{
		return;
	}
	FQuat TargetRotation = Direction.ToOrientationQuat();
	FQuat PlayerRotation = FQuat::Slerp(VisualShape->GetForwardVector().ToOrientationQuat(), TargetRotation, RotationSpeed * GetWorld()->DeltaTimeSeconds);
	
	//GetOwner()->SetActorRotation(PlayerRotation);
	VisualShape->SetWorldRotation(PlayerRotation);

}

void UFG_LocomotionComponent::Jump(UFG_DA_JumpStats* JumpStats)
{
	JumpRAW(JumpStats->JumpForce, JumpStats->RayDistance, JumpStats->DisableFloatingTime);
}

void UFG_LocomotionComponent::JumpRAW(float JumpForce, float RayDistance, float DisableFloatingTime)
{
	DisableFloat(DisableFloatingTime); //Briefly disable floating because otherwise the damping hover force will make the jump inconsistently high.

	JumpRAW(JumpForce, RayDistance);
}

void UFG_LocomotionComponent::JumpRAW(float JumpForce, float RayDistance)
{
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

bool UFG_LocomotionComponent::HasJustJumped() const
{
	return DisabledFloatTimer > 0.0f;
}

FVector UFG_LocomotionComponent::GetInputRelativeToCamera(const FVector InputVector, const USceneComponent* CameraTransform)
{
	FVector NewVector = CameraTransform->GetRightVector() * InputVector.X + CameraTransform->GetForwardVector() * InputVector.Y;
	NewVector.Z = 0;
	NewVector.Normalize();
	return NewVector;
}

FVector UFG_LocomotionComponent::GetWallClimbNormal()
{
	return WallClimbNormal;
}

