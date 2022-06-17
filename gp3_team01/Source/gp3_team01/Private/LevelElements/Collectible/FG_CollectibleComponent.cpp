// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Collectible/FG_CollectibleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "Audio/DA_FG_NoteSheet.h"
#include "Audio/InstrumentChord.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/FG_DA_Instrument.h"
#include <algorithm>

// Sets default values for this component's properties
UFG_CollectibleComponent::UFG_CollectibleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFG_CollectibleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFG_CollectibleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_CollectibleComponent::HideSelf(bool StillVisislbe) const
{
	AActor* Self = GetOwner();
	Self->SetActorHiddenInGame(!StillVisislbe);
	Self->SetActorEnableCollision(false);
	Self->SetActorTickEnabled(false);
}

void UFG_CollectibleComponent::OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State)
{
	if (State == EFG_TouchingState::StartedTouching)
	{
		AFG_PlayerCharacter* PlayerCharacter = Cast<AFG_PlayerCharacter>(LocomotionComponentOfCaller->GetOwner());
		if (ensure(PlayerCharacter != nullptr))
		{
			UInstrumentChord* Chord = NoteSheet->GetCurrent();
			PlayerCharacter->InstrumentUser->PlayChord(Chord, 1.0f, 1.0f, 0);
			NoteSheet->SetCurrent(Chord->Next(0));

			PlayRandomBellSound(PlayerCharacter);
			OnCollect(PlayerCharacter);
		}
	}
}

void UFG_CollectibleComponent::OnGliderTouch_Implementation(UFG_GliderComponent* GliderComponentOfCaller, EFG_TouchingState State)
{
	if (State == EFG_TouchingState::StartedTouching)
	{
		AFG_PlayerCharacter* PlayerCharacter = Cast<AFG_PlayerCharacter>(GliderComponentOfCaller->GetOwner());
		if (ensure(PlayerCharacter != nullptr))
		{
			OnCollect(PlayerCharacter);
			UInstrumentChord* Chord = NoteSheet->GetCurrent();
			PlayerCharacter->InstrumentUser->PlayChord(Chord, 1.0f, 1.0f, 0);
			NoteSheet->SetCurrent(Chord->Next(0));

			PlayRandomBellSound(PlayerCharacter);
			OnCollect(PlayerCharacter);
		}
	}
}

void UFG_CollectibleComponent::PlayRandomBellSound(AFG_PlayerCharacter* PlayerCharacter)
{
	static TArray<int> RandomIndeces;
	if (RandomIndeces.Num() == 0)
	{
		for (int i = 0; i < Bells->Num(); i++)
		{
			RandomIndeces.Push(i);
		}
		std::random_shuffle(RandomIndeces.GetData(), RandomIndeces.GetData() + RandomIndeces.Num());
	}
	const int RandomIndex = RandomIndeces.Pop(false);
	PlayerCharacter->InstrumentUser->PlayChord(Bells->Find(RandomIndex), 0.20f, 1.0f, 1);
}

