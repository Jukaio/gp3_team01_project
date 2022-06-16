// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Yak/FG_YakAIController.h"
#include "LevelElements/Yak/FG_YakAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AFG_YakAIController::AFG_YakAIController()
{
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AFG_YakAIController::OnPerceptionUpdate);

}

void AFG_YakAIController::OnPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (AFG_YakAICharacter* PlayerCharacter = Cast<AFG_YakAICharacter>(Actor))
	{
		if (BlackboardComp == nullptr)
		{
			BlackboardComp = GetBlackboardComponent();
		}

		if (Stimulus.WasSuccessfullySensed())
		{
			//Stimulus was found
			BlackboardComp->SetValueAsObject("Player", Actor);
		}

		//Stimulus was lost
		BlackboardComp->ClearValue("Player");
	}

}

void AFG_YakAIController::BeginPlay()
{
	Super::BeginPlay();
	BlackboardComp = GetBlackboardComponent();
}