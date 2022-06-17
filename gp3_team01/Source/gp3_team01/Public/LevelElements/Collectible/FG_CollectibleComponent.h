// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "Player/Gliding/FG_GliderResponse.h"
#include "FG_CollectibleComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP3_TEAM01_API UFG_CollectibleComponent : public UActorComponent, public IFG_LocomotionResponse, public IFG_GliderResponse
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_CollectibleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCollect(class AFG_PlayerCharacter* PlayerCharacter);

	UFUNCTION(BlueprintCallable)
	void HideSelf(bool StillVisislbe) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDA_FG_IntegerData* Score;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDA_FG_NoteSheet* NoteSheet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UFG_DA_Instrument* Bells;


	virtual void OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State) override;
	virtual void OnGliderTouch_Implementation(UFG_GliderComponent* GliderComponentOfCaller, EFG_TouchingState State) override;

private:
	void PlayRandomBellSound(class AFG_PlayerCharacter* PlayerCharacter);
};
