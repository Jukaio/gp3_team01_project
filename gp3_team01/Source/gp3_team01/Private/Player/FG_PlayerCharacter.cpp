// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/Input/FG_InputWriteComponent.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_SFSM.h"

// Sets default values
AFG_PlayerCharacter::AFG_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;
	InputWriter = CreateDefaultSubobject<UFG_InputWriteComponent>(TEXT("InputWriter"));
	LocomotionComp = CreateDefaultSubobject<UFG_LocomotionComponent>(TEXT("LocomotionComponent"));
	Glider = CreateDefaultSubobject<UFG_GliderComponent>(TEXT("Glider"));

	LocomotionCoreState = CreateDefaultSubobject<UFG_LocomotionPlayerState>(TEXT("LocomotionPlayerState"));
	GlidingCoreState = CreateDefaultSubobject<UFG_GlidingPlayerState>(TEXT("GlidingPlayerState"));
}

// Called when the game starts or when spawned
void AFG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	StateMachine = NewObject<UFG_SFSM>();
	StateMachine->Push(LocomotionCoreState);
}

// Called every frame
void AFG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StateMachine->OnStateMachineTick(DeltaTime);

	Stats->InputStats->UpdateBuffers();
}


