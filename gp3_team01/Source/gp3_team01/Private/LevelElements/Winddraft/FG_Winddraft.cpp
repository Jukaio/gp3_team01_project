// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelElements/Winddraft/FG_Winddraft.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"

// Sets default values
AFG_Winddraft::AFG_Winddraft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxTrigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFG_Winddraft::BeginPlay()
{
	Super::BeginPlay();

}

void AFG_Winddraft::ApplyWindForce(UPrimitiveComponent* ActorCapsuleComponent, AFG_PlayerCharacter* Player)
{
	UFG_GliderComponent* GliderComponent = Player->Glider;
	UE_LOG(LogTemp, Warning, TEXT("Overlap"))

	if (Player->StateMachine->TopIsEqual(Player->GlidingCoreState))
	{
		GliderComponent->ApplyWindForce(GetActorUpVector(), WindSpeedGliding);
	}
	else
	{
		ActorCapsuleComponent->AddForce(GetActorUpVector() * WindSpeedNotGliding);
	}

}

void AFG_Winddraft::OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State)
{
	if (State == EFG_TouchingState::StillTouching)
	{
		AFG_PlayerCharacter* Player = Cast<AFG_PlayerCharacter>(LocomotionComponentOfCaller->GetOwner());
		if (Player == nullptr)
		{
			return;
		}
		ApplyWindForce(Player->Collider, Player);
	}
}

void AFG_Winddraft::OnGliderTouch_Implementation(UFG_GliderComponent* GliderComponentOfCaller, EFG_TouchingState State)
{
	if (State == EFG_TouchingState::StillTouching)
	{
		AFG_PlayerCharacter* Player = Cast<AFG_PlayerCharacter>(GliderComponentOfCaller->GetOwner());
		if (Player == nullptr)
		{
			return;
		}
		ApplyWindForce(Player->Collider, Player);
	}
}

