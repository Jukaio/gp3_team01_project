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

	UFUNCTION(BlueprintCallable)
	void Bounce(class UPrimitiveComponent* OtherActor);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxTrigger;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = BouncePad)
	float BounceForce = 50000.f;

};
