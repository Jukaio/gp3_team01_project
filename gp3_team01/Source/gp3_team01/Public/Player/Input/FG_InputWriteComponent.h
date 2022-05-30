// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_InputWriteComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_InputWriteComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFG_InputWriteComponent();

	void HandleHorizontal(float value);
	void HandleVertical(float value);
	void HandleJumpReleased();
	void HandleJumpPressed();
	void HandleGlideReleased();
	void HandleGlidePressed();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UFG_DA_InputStats* InputStats;

	UPROPERTY()
	UInputComponent* InputComponent;
};
