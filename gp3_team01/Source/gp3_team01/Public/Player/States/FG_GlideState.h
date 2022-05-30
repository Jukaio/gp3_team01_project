// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/States/FG_PlayerState.h"
#include "FG_GlideState.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_GlideState : public UFG_PlayerState
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_GlideState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnStateEnter_Implementation() final;
	virtual void OnStateExit_Implementation() final;
	virtual bool OnStateTick_Implementation(float DeltaTime) final;
};
