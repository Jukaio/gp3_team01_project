// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FG_DA_InputStats.generated.h"

/**
 * 
 */
UCLASS()
class GP3_TEAM01_API UFG_DA_InputStats : public UDataAsset
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void SetRawInputVector(float x, float y);

	UFUNCTION(BlueprintCallable)
	void SetCameraForwardVector(FVector FwdVector);

	UFUNCTION(BlueprintCallable)
	void SetCameraNormalisedInputVector(float x, float y);

	UFUNCTION(BlueprintCallable)
	void SetIsJumping(bool value);

	UFUNCTION(BlueprintCallable)
	void SetIsGliding(bool value);

	UFUNCTION(BlueprintPure)
	FVector GetRawInputVector() const;

	UFUNCTION(BlueprintPure)
	FVector GetCameraForwardVector() const;

	UFUNCTION(BlueprintPure)
	FVector GetCameraNormalisedInputVector() const;

	UFUNCTION(BlueprintPure)
	bool GetIsJumpingPressed() const;

	UFUNCTION(BlueprintPure)
	bool GetIsGlidingPressed() const;

	UFUNCTION(BlueprintPure)
	bool GetWasJumpingJustPressed() const;
	
	UFUNCTION(BlueprintPure)
	bool GetWasGlidingJustPressed() const;

	UFUNCTION(BlueprintPure)
	bool GetWasJumpingJustReleased() const;

	UFUNCTION(BlueprintPure)
	bool GetWasGlidingJustReleased() const;

	UFUNCTION(BlueprintCallable)
	void InvalidateBuffers();

	UFUNCTION(BlueprintCallable)
	void UpdateBuffers();

private:
	FVector RawInputVector{ 0.0f, 0.0f, 0.0f };
	FVector CameraForwardVector{ 0.0f, 0.0f, 0.0f };
	FVector CameraNormalisedInputVector{ 0.0f, 0.0f, 0.0f };
	bool bJumpingPressedBuffer[2];
	bool bGlidingPressedBuffer[2];
};
