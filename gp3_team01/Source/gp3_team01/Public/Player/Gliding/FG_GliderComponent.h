// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "FG_GliderComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_GliderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_GliderComponent();
	
	UPROPERTY()
	class UFG_LocomotionComponent* LocomotionComponent;
	
	UPROPERTY()
	class USceneComponent* PlayerBody;

	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	USpringArmComponent* SpringArmComponent;
	
	UPROPERTY()
	AFG_PlayerCharacter* PlayerCharacter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* GlidingTurnCurve;
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GravitationalForce;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GlidingForce = 100000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MinimumGlideMomentum = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TurningEfficiency = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TurningSpeed = 5.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float GliderRollSpeed = 750.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TurnHorizontal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TurnVertical;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_InputStats* InputStats;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsGliding;
	
	FVector GlideDirection;
	FVector GlideVelocity;
	FVector LookDirection;
	FVector CurrentVelocity;
	FVector PreviousVelocity = FVector::ZeroVector;
	FVector Input;
	
	FVector WindDirection;
	float DragForce;
	float GravitationalVelocity = 0.f;
	float MaxGravitationalVelocity = 1500.f;
	float GliderMinimumGlide = 0.05f;
	float WindDraftForce = 0.f;
	float MaxWindDraftForce = 0.f;
	FVector2D TimeTurned;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetRotation();
	void SetTimeTurned();
	void Glide();

	UFUNCTION(BlueprintCallable)
	bool CheckTouching(class UPrimitiveComponent* Component);

	bool CheckTouchingGround(float DistanceRay);
	
	void ApplyWindForce(FVector WindDirection, float WindDraftForce);
	void StartGliding();
	void StopGliding();
	void DrawDebug();

private:
	TSet<AActor*> LastCallResults;
	TSet<AActor*> CurrentCallResults;
};
