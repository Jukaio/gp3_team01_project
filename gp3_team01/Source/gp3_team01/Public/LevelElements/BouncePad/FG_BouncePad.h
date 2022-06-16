// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FG_BouncePad.generated.h"

UCLASS()
class GP3_TEAM01_API AFG_BouncePad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFG_BouncePad();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Bounce(UFG_LocomotionComponent* Locomotion, float RayDistance, float DisableFloatTimer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Instrument;
	UPROPERTY()
	class UInstrumentChord* CurrentChord;
	UPROPERTY()
	class UFG_InstrumentComponent* InstrumentHandler;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BouncePad)
	float BounceForce = 1200.f;

	float LastTimeBounced = 0.0f;
};
