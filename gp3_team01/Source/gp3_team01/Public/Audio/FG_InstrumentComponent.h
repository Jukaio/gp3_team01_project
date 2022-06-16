// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Sound/SoundWave.h" 
#include "FG_InstrumentComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_InstrumentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_InstrumentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TArray<class UAudioComponent*> AudioComponentPool;

	class UAudioComponent* BorrowAudioComponent();

	UFUNCTION(BlueprintCallable)
	class UInstrumentChord* PlayAndGetPrev(class UInstrumentChord* Chord, int32 Prev = 0);

	UFUNCTION(BlueprintCallable)
	class UInstrumentChord* PlayAndGetNext(class UInstrumentChord* Chord, int32 Next = 0);

	void PlayChord(const UInstrumentChord* Chord, float Volume = 1.0f, float Pitch = 1.0f, int Repeats = 2, float FadeOutTimeStart = 1.0f, float FadeOutTimeEnd = 0.0f);

	UPROPERTY();
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	float BeatsPerMinute = 120.0f;

	UPROPERTY(EditAnywhere)
	float BeatInterval = 0.25;

private:

	struct TimedSound
	{
		class USoundWave* Sound;
		float WorldTimePoint;
		float VolumeMultiplier;
		float PitchMultiplier;
		float FadeOutStartTimePoint;
		float FadeOutEndTimePoint;

		bool operator<(const TimedSound& Other)
		{
			return Other.WorldTimePoint < this->WorldTimePoint;
		}
	};
	struct FadeOutSound
	{
		class UAudioComponent* AudioComponent;
		float InitialVolume;
		float FadeOutTimePointStart;
		float FadeOutTimePointDuration;
	};

	TArray<TimedSound> TimedSoundQueue;
	TArray<FadeOutSound> FadeOutQueue;
};
