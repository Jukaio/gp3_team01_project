#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Movement/FG_LocomotionResponse.h"
#include "FG_MovingPlatform.generated.h"

UCLASS()
class AFG_MovingPlatform : public AActor, public IFG_LocomotionResponse
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFG_MovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Moving Platform")
	float Speed = 200.f;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	FVector EndLocation;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Moving Platform", Meta = (MakeEditWidget = true))
	FVector EditableEndLocation;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Moving Platform")
	FVector DeltaMovementVector;

	UPROPERTY()
	bool IsMovingToEnd = true;

};