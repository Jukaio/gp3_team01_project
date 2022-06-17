// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "FG_CollectibleUnlocker.generated.h"


UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP3_TEAM01_API UFG_CollectibleUnlocker : public UActorComponent, public IFG_LocomotionResponse
{
	GENERATED_BODY()

public:	
	UFG_CollectibleUnlocker();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface * UnlockedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* LockedMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FullCycleDuration = 3.0f;

	UPROPERTY()
	TArray<class UStaticMeshComponent*> MeshComponents;

	virtual void OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State);

	UPROPERTY()
	class UFG_CollectorComponent* CurrentCollector;

	struct FromToCollectible
	{
		class UFG_CollectibleComponent* Context;
		FVector From;
		FVector To;
		class UStaticMeshComponent* TargetMesh;
	};

	UPROPERTY()
	TArray<class UFG_CollectibleComponent*> UsedCollectibles;
	UPROPERTY()
	TArray<class UStaticMeshComponent*> UsedMeshComponents;

	UPROPERTY()
	class UFG_CollectibleComponent* TravellingCollectible = nullptr;
	UPROPERTY()
	class UStaticMeshComponent* TravellingMeshComponent = nullptr;


	FVector CollectibleFromPos{ };
	FQuat CollectibleFromRot{ };
	FVector CollectibleFromScale{ };

	float Timer = 0.0f;
};
