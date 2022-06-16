// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "FG_YakAIController.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GP3_TEAM01_API AFG_YakAIController : public AAIController
{
	GENERATED_BODY()

	AFG_YakAIController();


	UFUNCTION()
	void OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* PerceptionComp = nullptr;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp = nullptr;
	
};
