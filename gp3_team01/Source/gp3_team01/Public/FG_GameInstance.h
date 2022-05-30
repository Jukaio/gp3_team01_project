// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FG_GameInstance.generated.h"

/**
 * 
 */

class UCameraComponent;

UCLASS()
class GP3_TEAM01_API UFG_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Meta = (DisplayName = "GetGameInstance", WorldContext = "WorldContext"))
	static UFG_GameInstance* Get(UObject* WorldContext);

	void Init() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	UCameraComponent* GetMainCamera();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	FRotator GetCameraOrientation();
	
};
