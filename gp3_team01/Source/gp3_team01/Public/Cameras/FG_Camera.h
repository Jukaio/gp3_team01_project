// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FG_Camera.generated.h"

UCLASS(Blueprintable)
class GP3_TEAM01_API AFG_Camera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFG_Camera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditInstanceOnly)
	AActor* FollowActor;

	UPROPERTY(EditAnywhere)
	class UFG_DA_CameraStats* CameraStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_InputStats* InputStats;

	UFUNCTION(BlueprintCallable)
	void SetFollowActor(AActor* Actor);

private:
	void HandleCameraXRotation(float InputAxis);
	void HandleCameraYRotation(float InputAxis);
};