// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/FG_DA_Instrument.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"
#include "Audio/InstrumentChord.h"
#include "Sound/SoundCue.h"

UInstrumentChord* UFG_DA_Instrument::Find(int32 Index) const
{
	Index = FMath::Clamp(Index, 0, Num() - 1);
	return Chords[Index];
}

int32 UFG_DA_Instrument::Num() const
{
	return Chords.Num();
}
