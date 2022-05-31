// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Gliding/FG_GliderComponent.h"

#include <Player/FG_DA_InputStats.h>

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"

// Sets default values for this component's properties
UFG_GliderComponent::UFG_GliderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFG_GliderComponent::BeginPlay()
{
	Super::BeginPlay();
	LocomotionComponent = Cast<UFG_LocomotionComponent>(GetOwner()->GetComponentByClass(UFG_LocomotionComponent::StaticClass())); //TODO:: Decouple reference
	
	CapsuleComponent = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	StaticMeshComponent =  Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(CapsuleComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Glider components capsule component is nullptr"))
	}
	CurrentVelocity = FVector::ZeroVector;
}

void UFG_GliderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LookDirection = InputStats->GetCameraForwardVector();
	Input = InputStats->GetRawInputVector();
}

void UFG_GliderComponent::Glide()
{
	float LiftForceNormalized = FVector::UpVector | LookDirection;
	GlideDirection = FMath::Lerp(GlideDirection, LookDirection, 1 - FMath::Exp(-TurningSpeed * GetWorld()->DeltaTimeSeconds)).GetSafeNormal();
	
	CurrentVelocity += GlideDirection * -LiftForceNormalized * GlidingForce * GetWorld()->DeltaTimeSeconds;
	
	CurrentVelocity = FMath::Lerp( CurrentVelocity, GlideDirection * CurrentVelocity.Size(), 1 - FMath::Exp(-TurningEfficiency * GetWorld()->DeltaTimeSeconds)) * (1 + (CurrentVelocity.GetSafeNormal() | GlideDirection)) / 2;
	GravitationalForce = CurrentVelocity.Size() < MinimumGlideMomentum ? GravitationalForce + GravityForce * GravityForce * GetWorld()->DeltaTimeSeconds : GravitationalForce - GravityForce * GravityForce * GetWorld()->DeltaTimeSeconds;
	GravitationalForce = FMath::Clamp(GravitationalForce, 0.f, MaxGravitationalVelocity);
	FVector Momentum = CapsuleComponent->GetPhysicsLinearVelocity();
	
	Momentum = FMath::Lerp( Momentum, GlideDirection * Momentum.Size(), 1 - FMath::Exp(-1.f * GetWorld()->DeltaTimeSeconds )) ;
	
	FVector DeltaVelocity = CurrentVelocity - PreviousVelocity;
	
	CapsuleComponent->SetPhysicsLinearVelocity(DeltaVelocity + Momentum + GravitationalForce * FVector::DownVector);
	PreviousVelocity = CurrentVelocity;
	
	FQuat TargetRotation = CapsuleComponent->GetPhysicsLinearVelocity().ToOrientationQuat();
	FQuat PlayerRotation = FQuat::Slerp(  GetOwner()->GetActorForwardVector().ToOrientationQuat(), TargetRotation, 50.f * GetWorld()->DeltaTimeSeconds);
	GetOwner()->SetActorRotation(PlayerRotation);
}

void UFG_GliderComponent::GlideAxisControlled()
{
	FVector InputVector = FVector::RightVector * Input.X;
	
	GlideDirection = FMath::Lerp(GlideDirection, LookDirection, 1 - FMath::Exp(-TurningSpeed * GetWorld()->DeltaTimeSeconds)).GetSafeNormal();
	float LiftForceNormalized = FVector::UpVector | GlideDirection;
	CurrentVelocity += GlideDirection * -LiftForceNormalized * GlidingForce * GetWorld()->DeltaTimeSeconds;
	
	CurrentVelocity = FMath::Lerp( CurrentVelocity, GlideDirection * CurrentVelocity.Size(), 1 - FMath::Exp(-TurningEfficiency * GetWorld()->DeltaTimeSeconds));
	GravitationalForce = CurrentVelocity.Size() < MinimumGlideMomentum ? GravitationalForce + GravityForce * GravityForce * GetWorld()->DeltaTimeSeconds : GravitationalForce - GravityForce * GravityForce * GetWorld()->DeltaTimeSeconds;

	FVector Momentum = CapsuleComponent->GetPhysicsLinearVelocity();

	Momentum = FMath::Lerp( Momentum, GlideDirection * Momentum.Size(), 1 - FMath::Exp(-1.f * GetWorld()->DeltaTimeSeconds));
	GravitationalForce = FMath::Clamp(GravitationalForce, 0.f, MaxGravitationalVelocity);
	FVector DeltaVelocity = CurrentVelocity - PreviousVelocity;
	
	CapsuleComponent->SetPhysicsLinearVelocity(DeltaVelocity + Momentum );
	PreviousVelocity = CurrentVelocity;
}

void UFG_GliderComponent::Boost(FVector Boost)
{
	CurrentVelocity += Boost;
	
}

void UFG_GliderComponent::DrawDebug()
{
	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), GlideDirection * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Blue);
	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), LookDirection * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Green);

	DrawDebugDirectionalArrow(GetWorld(), CapsuleComponent->GetComponentLocation(), CapsuleComponent->GetPhysicsLinearVelocity() * 500.f + CapsuleComponent->GetComponentLocation(), 350.f, FColor::Green);
}


