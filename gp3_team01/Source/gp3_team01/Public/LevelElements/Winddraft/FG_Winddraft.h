// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FG_Winddraft.generated.h"

class UBoxComponent;

UCLASS()
class GP3_TEAM01_API AFG_Winddraft : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFG_Winddraft();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxTrigger;

	UPROPERTY()
	class AFG_PlayerCharacter* ThePlayer;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Winddraft)
	float WindSpeedGliding = 3000000.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Winddraft)
	float WindSpeedNotGliding = 60000.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Winddraft)
	bool IsWinddraftOn = true;

	UPROPERTY()
	bool IsApplyingForceOnPlayer = false;

	UFUNCTION(BlueprintCallable)
	void ApplyWindForce(class UPrimitiveComponent* OtherActor, AFG_PlayerCharacter* Player);

};
