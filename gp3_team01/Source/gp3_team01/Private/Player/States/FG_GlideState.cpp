// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/FG_GlideState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UFG_GlideState::UFG_GlideState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFG_GlideState::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}


// Called every frame
void UFG_GlideState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_GlideState::OnStateEnter_Implementation()
{
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	CapsuleCollider->SetEnableGravity(false);
}

void UFG_GlideState::OnStateExit_Implementation()
{
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;

	CapsuleCollider->SetEnableGravity(true);
}

bool UFG_GlideState::OnStateTick_Implementation(float DeltaTime)
{
	UFG_GliderComponent* Glider = FGPlayerCharacter->Glider;
	Glider->Glide();

	const bool IsGlidingPressed = FGPlayerCharacter->Stats->InputStats->GetIsGlidingPressed();
	//UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
	
	return IsGlidingPressed; 
}

