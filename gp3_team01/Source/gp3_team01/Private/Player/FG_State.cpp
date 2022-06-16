// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FG_State.h"

// Add default functionality here for any IFG_State functions that are not pure virtual.

void IFG_State::OnStateEnter_Implementation()
{
}

bool IFG_State::OnStateTick_Implementation(float DeltaTime)
{
	return false;
}

void IFG_State::OnStateExit_Implementation()
{
}

void IFG_State::OnStatePush_Implementation()
{
}

void IFG_State::OnStatePop_Implementation()
{
}
