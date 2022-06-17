// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Gliding/FG_GliderComponent.h"

#include <Player/FG_DA_InputStats.h>

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Gliding/FG_GliderResponse.h"
#include "FG_BroadcastTemplate.h"
#include "Cameras/FG_Camera.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UFG_GliderComponent::UFG_GliderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UFG_GliderComponent::BeginPlay()
{
	Super::BeginPlay();
	SpringArmComponent = Cast<USpringArmComponent>(GetOwner()->GetComponentByClass(USpringArmComponent::StaticClass()));
	CapsuleComponent = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	StaticMeshComponent =  Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	PlayerCharacter = CastChecked<AFG_PlayerCharacter>(GetOwner());
	PlayerBody = PlayerCharacter->MeshPivot;
	if(CapsuleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Glider components capsule component is nullptr"))
	}
	CurrentVelocity = FVector::ZeroVector;
}
void UFG_GliderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Input = InputStats->GetRawInputVector();
	
	LookDirection = PlayerCharacter->GetMyCamera()->Camera->GetForwardVector();
}
void UFG_GliderComponent::SetRotation()
{
	
}
void UFG_GliderComponent::SetTimeTurned()
{
	float DeltaTime = GetWorld()->DeltaTimeSeconds;
	TimeTurned.X = Input.X != 0.f ? TimeTurned.X + DeltaTime * FMath::Abs(Input.X) : TimeTurned.X - DeltaTime;
	TimeTurned.Y = Input.Y != 0.f ? TimeTurned.Y + DeltaTime * FMath::Abs(Input.Y) : TimeTurned.Y - DeltaTime;
	
	TimeTurned.X = FMath::Clamp(TimeTurned.X, 0.f, TimeTurned.X);
	TimeTurned.Y = FMath::Clamp(TimeTurned.Y, 0.f, TimeTurned.Y);
}

void UFG_GliderComponent::Glide()
{
	FQuat NewRotation = FQuat::Slerp( PlayerBody->GetComponentQuat(), (LookDirection + CapsuleComponent->GetPhysicsLinearVelocity().GetSafeNormal() + WindDirection * WindDraftForce).ToOrientationQuat(), TurningSpeed * GetWorld()->DeltaTimeSeconds);
	GlideDirection = PlayerBody->GetForwardVector();
	
	PlayerBody->SetWorldRotation(NewRotation);
	
	float Roll = PlayerCharacter->GetMyCamera()->Camera->GetRightVector() | CapsuleComponent->GetPhysicsLinearVelocity().GetSafeNormal();
	float Pitch = PlayerCharacter->GetMyCamera()->Camera->GetUpVector() | CapsuleComponent->GetPhysicsLinearVelocity().GetSafeNormal();
	
	FRotator GliderRotation = FRotator(0.f, 0.f,   FMath::Clamp( -Roll * GliderRollSpeed * FMath::Clamp(CurrentVelocity.Size() / 1000.f + 1.f, 1.f, 2.f) * GetWorld()->DeltaTimeSeconds, -15.f, 15.f));
	PlayerBody->SetWorldRotation(PlayerBody->GetComponentRotation() + GliderRotation);
	float LiftForceNormalized = FVector::UpVector | GlideDirection;
	float DeltaTime = GetWorld()->DeltaTimeSeconds;
	CurrentVelocity += WindDirection * WindDraftForce * DeltaTime;
	CurrentVelocity += GlideDirection * -LiftForceNormalized * GlidingForce * DeltaTime;
	CurrentVelocity = FMath::Lerp( CurrentVelocity, GlideDirection * CurrentVelocity.Size(),
		1 - FMath::Exp(-TurningEfficiency * DeltaTime)) * (1 + (CurrentVelocity.GetSafeNormal() | GlideDirection)) / 2;
	
	GravitationalVelocity = CurrentVelocity.Size() < MinimumGlideMomentum ? GravitationalVelocity + GravitationalForce * GravitationalForce * DeltaTime : 0.f;

	GravitationalVelocity = FMath::Clamp(GravitationalVelocity, 0.f, MaxGravitationalVelocity);
	FVector Momentum = CapsuleComponent->GetPhysicsLinearVelocity();

	Momentum = FMath::Lerp( Momentum, GlideDirection * Momentum.Size(), 1 - FMath::Exp(-1.f * DeltaTime )) ;

	FVector DeltaVelocity = CurrentVelocity - PreviousVelocity;

	CapsuleComponent->SetPhysicsLinearVelocity(DeltaVelocity + Momentum + GravitationalVelocity * FVector::DownVector + FVector::DownVector * 1.f);
	PreviousVelocity = CurrentVelocity;
	
	WindDraftForce = FMath::Clamp(WindDraftForce - MaxWindDraftForce * 2 * DeltaTime, 0.f, WindDraftForce);
	
}

void UFG_GliderComponent::ApplyWindForce(FVector Direction, float Force)
{
	WindDirection = Direction;
	WindDraftForce += Force * GetWorld()->DeltaTimeSeconds * 10;
	MaxWindDraftForce = Force;
	WindDraftForce = FMath::Clamp(Force, 0.f, MaxWindDraftForce);
	PlayerCharacter->GetMyCamera()->SpringArm->SetWorldRotation(FQuat::Slerp (PlayerCharacter->GetMyCamera()->SpringArm->GetForwardVector().ToOrientationQuat(),
		(WindDraftForce * WindDirection).ToOrientationQuat(), 1.5f * GetWorld()->DeltaTimeSeconds));
}

void UFG_GliderComponent::StartGliding()
{
	PreviousVelocity = FVector::ZeroVector;
	CurrentVelocity = FVector::ZeroVector;
	GravitationalVelocity = 0.f;
	GlideDirection = GetOwner()->GetActorForwardVector();
}

bool UFG_GliderComponent::CheckTouching(UPrimitiveComponent* Component)
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
			BroadcastResponse<IFG_GliderResponse>(Element, IFG_GliderResponse::Execute_OnGliderTouch, this, EFG_TouchingState::StartedTouching);
		}
	}

	for (auto& Result : Results)
	{
		AActor* Actor = Result.GetActor();
		BroadcastResponse<IFG_GliderResponse>(Actor, IFG_GliderResponse::Execute_OnGliderTouch, this, EFG_TouchingState::StillTouching);
	}

	for (auto& Element : LastCallResults)
	{
		if (!CurrentCallResults.Contains(Element)) {
			BroadcastResponse<IFG_GliderResponse>(Element, IFG_GliderResponse::Execute_OnGliderTouch, this, EFG_TouchingState::StoppedTouching);
		}
	}

	LastCallResults.Reset();
	LastCallResults = CurrentCallResults;
	return HasResults;
}

bool UFG_GliderComponent::CheckTouchingGround(float DistanceRay)
{
	FVector RayDown = GetOwner()->GetActorLocation() - FVector(0, 0, DistanceRay);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	TArray<AActor*> IgnoredActors;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));


	TArray<FHitResult> hits;
	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), CapsuleComponent->GetComponentLocation(), RayDown, 20.f, ObjectTypesArray, true, IgnoredActors, EDrawDebugTrace::ForOneFrame, hits, true))
	{
		GEngine->AddOnScreenDebugMessage
		(
			INDEX_NONE,
			0.5f,
			FColor::Yellow,
			FString::Printf(TEXT("Landed"))
		);
		for (auto& hit : hits)
		{
			BroadcastResponse<IFG_GliderResponse>(hit, IFG_GliderResponse::Execute_OnGliderLand, this);
		}
		return true;
	}
	return false;
}



void UFG_GliderComponent::StopGliding()
{
	PreviousVelocity = FVector::ZeroVector;
	CurrentVelocity = FVector::ZeroVector;
	GravitationalVelocity = 0.f;
	GlideDirection = GetOwner()->GetActorForwardVector();
	WindDirection = FVector::ZeroVector;
	WindDraftForce = 0.f;
	
}

void UFG_GliderComponent::DrawDebug()
{
	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), GlideDirection * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Blue);
	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), LookDirection * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Green);

	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetPhysicsLinearVelocity() * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Green);
}


