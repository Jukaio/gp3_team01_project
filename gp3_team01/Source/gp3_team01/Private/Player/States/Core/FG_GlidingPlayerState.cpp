// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Gliding/FG_GlidingIdleSubState.h"
#include "Player/States/Gliding/FG_GlidingSteeringSubState.h"

// Sets default values for this component's properties
UFG_GlidingPlayerState::UFG_GlidingPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	GlidingIdleSubState = CreateDefaultSubobject<UFG_GlidingIdleSubState>(TEXT("GlidingIdleState"));
	GlidingSteeringSubState = CreateDefaultSubobject<UFG_GlidingSteeringSubState>(TEXT("GlidingSteeringState"));
	// ...
}


// Called when the game starts
void UFG_GlidingPlayerState::BeginPlay()
{
	Super::BeginPlay();
	GliderComponent = FGPlayerCharacter->Glider;
	FGPlayerCharacter->Collider->OnComponentHit.AddDynamic(this, &UFG_GlidingPlayerState::OnCollision);
	SubStateMachine = NewObject<UFG_SFSM>();
}


// Called every frame
void UFG_GlidingPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_GlidingPlayerState::OnStateEnter_Implementation()
{
	SubStateMachine->Push(GlidingIdleSubState);

	GliderComponent->PreviousVelocity = FVector::ZeroVector;
	GliderComponent->CurrentVelocity = FVector::ZeroVector;
	GliderComponent->GravitationalForce = 0.f;
	GliderComponent->GlideDirection = FGPlayerCharacter->GetActorForwardVector();
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	CapsuleCollider->SetEnableGravity(false);
	bIsColliding = false;
}

void UFG_GlidingPlayerState::OnStateExit_Implementation()
{
	SubStateMachine->Clear();
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	GliderComponent->PreviousVelocity = FVector::ZeroVector;
	GliderComponent->CurrentVelocity = FVector::ZeroVector;
	GliderComponent->GravitationalForce = 0.f;
	GliderComponent->GlideDirection = FGPlayerCharacter->GetActorForwardVector();
	CapsuleCollider->SetEnableGravity(true);
}

bool UFG_GlidingPlayerState::OnStateTick_Implementation(float DeltaTime)
{
	SubStateMachine->OnStateMachineTick(DeltaTime);
	GliderComponent->Glide();
	const bool IsGlidingPressed = FGPlayerCharacter->Stats->InputStats->GetIsGlidingPressed();
	
	if(bIsColliding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exited glide state"));
		return false;
	}
	return IsGlidingPressed;
	//UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
}

void UFG_GlidingPlayerState::OnCollision(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	bIsColliding = true;
	UE_LOG(LogTemp, Warning, TEXT("Start collision"));
}

