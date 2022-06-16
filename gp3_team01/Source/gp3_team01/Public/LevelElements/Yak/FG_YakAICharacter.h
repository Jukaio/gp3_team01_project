// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FG_YakAICharacter.generated.h"

UCLASS()
class GP3_TEAM01_API AFG_YakAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFG_YakAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Bounce(UFG_LocomotionComponent* Locomotion, float RayDistance, float DisableFloatTimer);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Instrument;
	UPROPERTY()
	class UInstrumentChord* CurrentChord;
	UPROPERTY()
	class UFG_InstrumentComponent* InstrumentHandler;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BouncePad)
	float BounceForce = 1200.f;

	float LastTimeBounced = 0.0f;

};
