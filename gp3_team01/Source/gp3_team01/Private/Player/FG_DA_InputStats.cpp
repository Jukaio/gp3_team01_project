// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FG_DA_InputStats.h"

void UFG_DA_InputStats::SetRawInputVector(float x, float y)
{
	RawInputVector = { x, y, 0.0f };
}

void UFG_DA_InputStats::SetCameraForwardVector(FVector FwdVector)
{
	CameraForwardVector = FwdVector;
}

void UFG_DA_InputStats::SetCameraNormalisedInputVector(float x, float y)
{
	CameraNormalisedInputVector = { x, y, 0.0f };
}

void UFG_DA_InputStats::SetIsJumping(bool value)
{
	bJumpingPressedBuffer[0] = value;
}

void UFG_DA_InputStats::SetIsGliding(bool value)
{
	bGlidingPressedBuffer[0] = value;
}

FVector UFG_DA_InputStats::GetRawInputVector() const
{
	return RawInputVector;
}

FVector UFG_DA_InputStats::GetCameraForwardVector() const
{
	return CameraForwardVector;
}

FVector UFG_DA_InputStats::GetCameraNormalisedInputVector() const
{
	return CameraNormalisedInputVector;
}

bool UFG_DA_InputStats::GetIsJumpingPressed() const
{
	return bJumpingPressedBuffer[0];
}

bool UFG_DA_InputStats::GetIsGlidingPressed() const
{
	return bGlidingPressedBuffer[0];
}

bool UFG_DA_InputStats::GetWasJumpingJustPressed() const
{
	return bJumpingPressedBuffer[0] && !bJumpingPressedBuffer[1];
}

bool UFG_DA_InputStats::GetWasGlidingJustPressed() const
{
	return bGlidingPressedBuffer[0] && !bGlidingPressedBuffer[1];;
}

bool UFG_DA_InputStats::GetWasJumpingJustReleased() const
{
	return !bJumpingPressedBuffer[0] && bJumpingPressedBuffer[1];
}

bool UFG_DA_InputStats::GetWasGlidingJustReleased() const
{
	return !bGlidingPressedBuffer[0] && bGlidingPressedBuffer[1];
}

void UFG_DA_InputStats::InvalidateBuffers()
{
	RawInputVector = FVector::ZeroVector;
	CameraForwardVector = FVector::ZeroVector;
	CameraNormalisedInputVector = FVector::ZeroVector;
	memset(bJumpingPressedBuffer, false, sizeof(bJumpingPressedBuffer));
	memset(bGlidingPressedBuffer, false, sizeof(bGlidingPressedBuffer));
}

void UFG_DA_InputStats::UpdateBuffers()
{
	bGlidingPressedBuffer[1] = bGlidingPressedBuffer[0];
	bJumpingPressedBuffer[1] = bJumpingPressedBuffer[0];
}
