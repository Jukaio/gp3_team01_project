// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_FG_NoteSheet.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class GP3_TEAM01_API UDA_FG_NoteSheet : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UInstrumentChord* GetCurrent();

	UFUNCTION(BlueprintCallable)
	void SetCurrent(UInstrumentChord* Next);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInstrumentChord* CurrentNote = nullptr;

};
