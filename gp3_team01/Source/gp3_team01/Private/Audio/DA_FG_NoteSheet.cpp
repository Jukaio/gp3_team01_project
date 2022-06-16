// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/DA_FG_NoteSheet.h"

UInstrumentChord* UDA_FG_NoteSheet::GetCurrent()
{
	return CurrentNote;
}

void UDA_FG_NoteSheet::SetCurrent(UInstrumentChord* Next)
{
	CurrentNote = Next;
}
