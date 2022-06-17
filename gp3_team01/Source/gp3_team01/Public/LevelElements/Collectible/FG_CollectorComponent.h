// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_CollectorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_CollectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_CollectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Add(class UFG_CollectibleComponent* Actor);

	UFUNCTION(BlueprintCallable)
	void Remove(class UFG_CollectibleComponent* Actor);

	UFUNCTION(BlueprintPure)
	int Num() const;

	UFUNCTION(BlueprintCallable)
	class UFG_CollectibleComponent* Pop();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Distance;

	UPROPERTY()
	class UCapsuleComponent* Capsule;
	
	UPROPERTY()
	class AFG_PlayerCharacter* PlayerCharacter;

	UPROPERTY()
	TArray<class UFG_CollectibleComponent*> CollectedActors;


	struct Data
	{
		FVector Origin;
		FVector NormalPlane;
		float From;
		float To;
		float Current;
	};

	TArray<Data> RotationData;
	TArray<float> Heights;
};
