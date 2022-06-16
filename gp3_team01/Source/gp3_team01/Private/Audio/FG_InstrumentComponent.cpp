// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/FG_InstrumentComponent.h"
#include "Components/AudioComponent.h"
#include "Audio/InstrumentChord.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/InstrumentChord.h"
#include <algorithm>

// Sets default values for this component's properties
UFG_InstrumentComponent::UFG_InstrumentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UFG_InstrumentComponent::BeginPlay()
{
	Super::BeginPlay();

	Root = Cast<USceneComponent>(GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass()));
	// ...
	
}

static FORCEINLINE float ConvertToDecibels(const float InLinear, const float InFloor = SMALL_NUMBER)
{
	return 20.0f * FMath::LogX(10.0f, FMath::Max(InLinear, InFloor));
}

// Function converts decibel to linear scale
static FORCEINLINE float ConvertToLinear(const float InDecibels)
{
	return FMath::Pow(10.0f, InDecibels / 20.0f);
}


// Called every frame
void UFG_InstrumentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	const float WorldTime = GetWorld()->TimeSeconds;


	TimedSoundQueue.RemoveAll([this, WorldTime](const TimedSound& Element) 
	{
		if (Element.WorldTimePoint <= WorldTime) 
		{
			UAudioComponent* AudioComponent = BorrowAudioComponent();
			
			AudioComponent->SetSound(Element.Sound);
			AudioComponent->SetPitchMultiplier(Element.PitchMultiplier);
			AudioComponent->VolumeMultiplier = Element.VolumeMultiplier;
			AudioComponent->Play();

			if (Element.FadeOutStartTimePoint < Element.FadeOutEndTimePoint) 
			{
				const float FadeOutTimePointStart = Element.Sound->Duration * Element.FadeOutStartTimePoint;
				const float FadeOutTimePointEnd = FadeOutTimePointStart + (Element.Sound->Duration * Element.FadeOutEndTimePoint);
				const float FadeOutTimePointDuration = FadeOutTimePointEnd - FadeOutTimePointStart;
				FadeOutQueue.Push({ AudioComponent, Element.VolumeMultiplier, WorldTime + FadeOutTimePointStart, FadeOutTimePointDuration });
			}
			return true;
		}
		return false;
	});

	FadeOutQueue.RemoveAll([this, WorldTime](const FadeOutSound& Element) 
	{
		if(Element.FadeOutTimePointStart + Element.FadeOutTimePointDuration <= WorldTime)
		{
			return true;
		}

		if (Element.FadeOutTimePointStart <= WorldTime)
		{
			const float CurrentFadeOutTimePoint = WorldTime - Element.FadeOutTimePointStart;
			const float NextVolume = FMath::Lerp(Element.InitialVolume, 0.0f, CurrentFadeOutTimePoint / Element.FadeOutTimePointDuration);
			Element.AudioComponent->SetVolumeMultiplier(NextVolume);
		}
		return false;
	});
}

UAudioComponent* UFG_InstrumentComponent::BorrowAudioComponent()
{
	if (AudioComponentPool.Num() == 0) {
		UAudioComponent* NewlyAddedComponent = NewObject<UAudioComponent>(this);
		NewlyAddedComponent->RegisterComponent();

		AudioComponentPool.Push(NewlyAddedComponent);
		NewlyAddedComponent->OnAudioFinishedNative.AddLambda([this](UAudioComponent* Audio) {
			AudioComponentPool.Push(Audio);
			FadeOutQueue.RemoveAll([&Audio, this](FadeOutSound& Element) 
			{
				return Element.AudioComponent == Audio;
			});
		});
	}

	UAudioComponent* AudioComponent = AudioComponentPool.Pop();
	return AudioComponent;
}

void UFG_InstrumentComponent::PlayChord(const UInstrumentChord* Chord, float Volume, float Pitch, int Repeats, float FadeOutTimeStart, float FadeOutTimeEnd)
{
	const float WorldTime = GetWorld()->TimeSeconds;
	const float Ratio = FMath::CeilToFloat(WorldTime / BeatInterval);
	const float TargetPlayTime = BeatInterval * Ratio;

	TArray<USoundWave*> Notes = Chord->Notes;

	if (Chord->bIsShufflingOnPlay)
	{
		std::random_shuffle(Notes.GetData(), Notes.GetData() + Notes.Num());
	}

	//float accumulator = 0.0f;
	float RandomBias = 0.0f;
	if (Chord->bIsPauseBetweenNotesSameForEachInstance == true)
	{
		RandomBias = FMath::FRandRange(Chord->NoteOffsetMin, Chord->NoteOffsetMax);
	}

	TArray<float> Accumulations;
	Accumulations.Add(0.0f);

	float MinFraction = 0.70f;
	float MaxFraction = 1.0f;
	{ // Play main sound
		const float MinRandom = FMath::Lerp(0.0f, Volume, MinFraction);
		const float MaxRandom = FMath::Lerp(0.0f, Volume, MaxFraction);

		for (auto& Note : Notes)
		{
			TimedSoundQueue.Add({ Note, TargetPlayTime + Accumulations.Last(), FMath::FRandRange(MinRandom, MaxRandom), Pitch, FadeOutTimeStart, FadeOutTimeEnd });
			if (Chord->bIsPauseBetweenNotesSameForEachInstance == false)
			{
				RandomBias = FMath::FRandRange(Chord->NoteOffsetMin, Chord->NoteOffsetMax);
			}
			Accumulations.Add(Accumulations.Last() + RandomBias);
		}
	}

	const int Count = Repeats;
	float fraction = MinFraction / Count;

	// Play echo
 	for (int index = 0; index < Count; index++)
	{
		for (int i = 0; i < Notes.Num(); i++)
		{
			auto& Note = Notes[i];
			const float Accumulation = Accumulations[i] + (BeatInterval * 1.5f);
			const float TargetWorldTime = TargetPlayTime + Accumulation;

			const float MinVolume = FMath::Lerp(0.0f, Volume, fraction * (Count - (index + 1)));
			const float MaxVolume = FMath::Lerp(0.0f, Volume, (fraction * (Count - index)));

			TimedSoundQueue.Add({ Note, TargetWorldTime, FMath::FRandRange(MinVolume, MaxVolume), Pitch, FadeOutTimeStart, FadeOutTimeEnd });
			Accumulations[i] = Accumulation;
		}
	}
}

UInstrumentChord* UFG_InstrumentComponent::PlayAndGetPrev(UInstrumentChord* Chord, int32 Prev)
{
	if (Chord == nullptr)
	{
		return nullptr;
	}

	UInstrumentChord* PrevChord = Chord->Next(Prev);
	PlayChord(PrevChord);
	return PrevChord;
}

UInstrumentChord* UFG_InstrumentComponent::PlayAndGetNext(UInstrumentChord* Chord, int32 Next)
{
	if (Chord == nullptr)
	{
		return nullptr;
	}
	UInstrumentChord* NextChord = Chord->Next(Next);
	PlayChord(NextChord);
	return NextChord;
}

