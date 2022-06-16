// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/InstrumentChord.h"

UInstrumentChord* UInstrumentChord::Next(int Index) const
{
	if (NextChords.Num() == 0)
	{
		return nullptr;
	}

	Index = FMath::Clamp(Index, 0, NextChords.Num() - 1);
	return NextChords[Index];
}

UInstrumentChord* UInstrumentChord::Prev(int Index) const
{
	if (PrevChords.Num() == 0)
	{
		return nullptr;
	}

	Index = FMath::Clamp(Index, 0, PrevChords.Num() - 1);
	return PrevChords[Index];
}
