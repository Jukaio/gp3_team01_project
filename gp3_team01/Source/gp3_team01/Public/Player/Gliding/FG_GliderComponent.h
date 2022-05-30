// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_GliderComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_GliderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_GliderComponent();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGliding;

	UPROPERTY()
	class UFG_LocomotionComponent* LocomotionComponent;

	UPROPERTY()
	class USceneComponent* SceneComponent;

	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GravityForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GlidingForce = 100000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinimumGlideMomentum = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TurningEfficiency = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TurningSpeed = 5;
	
	float GliderMinimumGlide = 0.05f;
	FVector GlideDirection;

	FVector GlideVelocity;
	float DragForce;
	FVector LookDirection;
	float GravitationalForce = 0.f;
	float MaxGravitationalVelocity = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_InputStats* InputStats;
	FVector CurrentVelocity;
	FVector Input;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Glide();
	void Boost(FVector Boost);
	
	void DrawDebug();
};
