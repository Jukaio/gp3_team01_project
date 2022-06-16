// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Sound/SoundWave.h" 
#include "InstrumentChord.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UInstrumentChord : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<USoundWave*> Notes;

	UPROPERTY(EditAnywhere)
	TArray<UInstrumentChord*> NextChords;

	UPROPERTY(EditAnywhere)
	TArray<UInstrumentChord*> PrevChords;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsShufflingOnPlay = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPauseBetweenNotesSameForEachInstance = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NoteOffsetMin = 0.02f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NoteOffsetMax = 0.06f;

	UFUNCTION(BlueprintPure)
	UInstrumentChord* Next(int Index) const;

	UFUNCTION(BlueprintPure)
	UInstrumentChord* Prev(int Index) const;
};
