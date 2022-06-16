// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/FG_State.h"
#include "FG_PlayerState.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_PlayerState : public UActorComponent, public IFG_State
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_PlayerState();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class AFG_PlayerCharacter* FGPlayerCharacter;

	UFUNCTION(BlueprintPure)
	class AFG_PlayerCharacter* GetFGPlayer() const;

	virtual void OnStateEnter_Implementation() override;
	virtual void OnStatePush_Implementation() override;
	virtual bool OnStateTick_Implementation(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	float GetTimeSinceEnter() const;

	UFUNCTION(BlueprintPure)
	float GetTimeSincePush() const;
private:
	float TimeSinceEnter;
	float TimeSincePush;
};
