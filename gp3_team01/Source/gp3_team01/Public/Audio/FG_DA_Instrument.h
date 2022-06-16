// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_Instrument.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GP3_TEAM01_API UFG_DA_Instrument : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<class UInstrumentChord*> Chords;

	class UInstrumentChord* Find(int32 Index) const;
	int32 Num() const;


};
