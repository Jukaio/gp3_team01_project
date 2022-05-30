// Fill out your copyright notice in the Description page of Project Settings.


#include "FG_GameInstance.h"
#include "Camera/CameraComponent.h"



UFG_GameInstance* UFG_GameInstance::Get(UObject* WorldContext)
{
	UWorld* World = WorldContext->GetWorld();
	return World->GetGameInstance<UFG_GameInstance>();
}

void UFG_GameInstance::Init()
{
	Super::Init();
	UFG_GameInstance::Get(this);
	UE_LOG(LogTemp, Log, TEXT("British programmers be like: init"));
}