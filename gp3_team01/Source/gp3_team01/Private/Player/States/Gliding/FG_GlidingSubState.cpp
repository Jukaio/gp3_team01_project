// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Gliding/FG_GlidingSubState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"

void UFG_GlidingSubState::BeginPlay()
{
	Super::BeginPlay();
	Context = CastChecked<UFG_GlidingPlayerState>(GetOwner()->GetComponentByClass(UFG_GlidingPlayerState::StaticClass()));
}

UFG_GlidingPlayerState* UFG_GlidingSubState::GetContext() const
{
	return Context;
}
