// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/FG_PlayerState.h"
#include "Player/FG_PlayerCharacter.h"
// Sets default values for this component's properties
UFG_PlayerState::UFG_PlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFG_PlayerState::BeginPlay()
{
	Super::BeginPlay(); 
	FGPlayerCharacter = CastChecked<AFG_PlayerCharacter>(GetOwner());
	// ...
	
}


// Called every frame
void UFG_PlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AFG_PlayerCharacter* UFG_PlayerState::GetFGPlayer() const
{
	return FGPlayerCharacter;
}

