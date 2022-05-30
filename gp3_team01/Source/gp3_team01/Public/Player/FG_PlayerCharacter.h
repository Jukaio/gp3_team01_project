// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FG_PlayerCharacter.generated.h"

UCLASS()
class GP3_TEAM01_API AFG_PlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFG_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public: //Components

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_InputWriteComponent* InputWriter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UFG_LocomotionComponent* LocomotionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_GliderComponent* Glider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_SFSM* StateMachine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCapsuleComponent* Collider;

public: //Public Variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_DA_PlayerStats* Stats;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_LocomotionPlayerState* LocomotionCoreState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UFG_GlidingPlayerState* GlidingCoreState;

private: //Private Variables



};
