// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelElements/MovingPlatform/FG_MovingPlatform.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFG_MovingPlatform::AFG_MovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

}


// Called when the game starts or when spawned
void AFG_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();
	EndLocation = EditableEndLocation + GetActorLocation();
}

// Called every frame
void AFG_MovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (IsMovingToEnd)
	{
		FVector Direction = EndLocation - GetActorLocation();
		Direction.Normalize();

		float DistanceToTarget = FVector::Distance(GetActorLocation(), EndLocation);
		if (DistanceToTarget > 10.f)
		{
			DeltaMovementVector = Direction * Speed * DeltaTime;
			AddActorWorldOffset(Direction * Speed * DeltaTime);
		}
		else
		{
			IsMovingToEnd = false;
		}

	}

	else
	{
		FVector Direction = StartLocation - GetActorLocation();
		Direction.Normalize();

		float DistanceToTarget = FVector::Distance(GetActorLocation(), StartLocation);
		if (DistanceToTarget > 10.f)
		{
			DeltaMovementVector = Direction * Speed * DeltaTime;
			AddActorWorldOffset(Direction * Speed * DeltaTime);

		}
		else
		{
			IsMovingToEnd = true;
		}
	}


}