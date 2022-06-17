// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Yak/FG_YakAICharacter.h"
#include "Components/BoxComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/FG_SFSM.h"

// Sets default values
AFG_YakAICharacter::AFG_YakAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxTrigger->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFG_YakAICharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Instrument != nullptr)
	{
		CurrentChord = Instrument->Find(0);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No instrument attached"));
	}
	InstrumentHandler = Cast<UFG_InstrumentComponent>(GetComponentByClass(UFG_InstrumentComponent::StaticClass()));
}

void AFG_YakAICharacter::Bounce(UFG_LocomotionComponent* Locomotion, float RayDistance, float DisableFloatTimer)
{
	const float WorldTime = GetWorld()->TimeSeconds;
	const float Difference = WorldTime - LastTimeBounced; // Current time - Last time
	const float Threshold = 1.0f;

	AFG_PlayerCharacter* Player = Cast<AFG_PlayerCharacter>(Locomotion->GetOwner());
	if (Difference > Threshold)
	{
		Player->LocomotionCoreState->SubStateMachine->Push(Player->LocomotionCoreState->LocomotionAirborneSubState);
   		Locomotion->JumpRAW(BounceForce, RayDistance, DisableFloatTimer);
		if (InstrumentHandler != nullptr)
		{
			InstrumentHandler->PlayChord(CurrentChord, 0.75, 1.0f, 0, 0.0f, 0.25f);
			CurrentChord = CurrentChord->Next(0);
		}
		LastTimeBounced = GetWorld()->TimeSeconds;
	}
}


// Called every frame
void AFG_YakAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFG_YakAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

