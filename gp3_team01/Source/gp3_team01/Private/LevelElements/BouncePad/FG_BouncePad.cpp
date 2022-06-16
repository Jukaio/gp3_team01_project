// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/BouncePad/FG_BouncePad.h"
#include "Components/BoxComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"

// Sets default values
AFG_BouncePad::AFG_BouncePad()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxTrigger->SetupAttachment(Root);
}

void AFG_BouncePad::BeginPlay()
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

void AFG_BouncePad::Bounce(UFG_LocomotionComponent* Locomotion, float RayDistance, float DisableFloatTimer)
{
	if (!Locomotion->HasJustJumped())
	{
		Locomotion->JumpRAW(BounceForce, RayDistance, DisableFloatTimer);
		if (InstrumentHandler != nullptr)
		{
			InstrumentHandler->PlayChord(CurrentChord, 0.75, 1.0f, 0, 0.0f, 0.25f);
			CurrentChord = CurrentChord->Next(0);
		}
	}

}

