// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Collectible/FG_CollectorComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "LevelElements/Collectible/FG_CollectibleComponent.h"

// Sets default values for this component's properties
UFG_CollectorComponent::UFG_CollectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UFG_CollectorComponent::BeginPlay()
{
	Super::BeginPlay();
	Capsule = Cast<UCapsuleComponent>(GetOwner()->GetComponentByClass(UCapsuleComponent::StaticClass()));
	check(Capsule != nullptr);

	const float Height = Capsule->GetScaledCapsuleHalfHeight() + Distance;
	const float Width = Capsule->GetScaledCapsuleRadius() + Distance;
	RotationData.Empty();
	//RotationData.Push( Data{ FVector::ZeroVector, FVector::UpVector, 0.0f, 180 });
	const float Start = FMath::DegreesToRadians(0.0f);
	const float End = FMath::DegreesToRadians(359.9f);
	//otationData.Push( Data{ FVector::ZeroVector, FVector(0.5f, 0.5f, 1.0f).GetSafeNormal(), Start, End });
	//RotationData.Push(Data{ FVector::ZeroVector, FVector(-0.5f, -0.5f, 1.0f).GetSafeNormal(), Start + PI, End + PI});
	//RotationData.Push( Data{ FVector::ZeroVector, FVector::UpVector, );
	//RotationData.Push( Data{ FVector::ZeroVector, FVector::UpVector, });
	//RotationData.Push( Data{ FVector::ZeroVector, FVector::UpVector, });
	//RotationData.Push( Data{ FVector::ZeroVector, FVector::UpVector, });
	// ...
	PlayerCharacter = Cast<AFG_PlayerCharacter>(GetOwner());
	//auto Test = Spots.Pop(false);
	//TestPosition.Push(Test);
	//UsedSpots.Push(Test);
	
	// 

	//Test = Spots.Pop(false);
	//TestPosition.Push(Test);
	//UsedSpots.Push(Test);

}

// Special Thanks to Johnathan, Shaun and Geof!
FVector Slerp(FVector start, FVector end, float percent)
{
	// Dot product - the cosine of the angle between 2 vectors.
	float dot = FVector::DotProduct(start, end);
	// Clamp it to be in the range of Acos()
	// This may be unnecessary, but floating point
	// precision can be a fickle mistress.
	dot = FMath::Clamp(dot, -1.0f, 1.0f);
	// Acos(dot) returns the angle between start and end,
	// And multiplying that by percent returns the angle between
	// start and the final result.
	float theta = FMath::Acos(dot) * percent;
	FVector RelativeVec = (end - start) * dot;
	RelativeVec.Normalize();
	// Orthonormal basis
	// The final result.
	return ((start * FMath::Cos(theta)) + (RelativeVec * FMath::Sin(theta)));
}

// Called every frame
void UFG_CollectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//const float Height = Capsule->GetScaledCapsuleHalfHeight() + Distance;
	//const float Width = Capsule->GetScaledCapsuleRadius() + Distance;
	//
	//
	const UWorld* World = GetWorld();
	AActor* Actor = GetOwner();
	const float Fraction = DeltaTime / 2.0f;
	const FTransform Center = Actor->GetTransform();
	for (int index = 0; index < RotationData.Num(); index++)
	{
		auto& D = RotationData[index];
		auto& A = CollectedActors[index];
		D.Current += Fraction;
		if (D.Current > 1.0f)
		{
			D.Current = 0.0f;
		}
		//auto From = UsedSpots[index];
		//auto To = UsedSpots[(index + 1) % TestPosition.Num()];
		FQuat Rotation(FVector::UpVector, D.From);
		FQuat Target(FVector::UpVector, D.To);
		FVector Dir = FQuat::SlerpFullPath(Rotation, Target, index % 2 ? D.Current : 1.0f - D.Current).Vector();
		FVector Pos = Actor->GetActorLocation() + (Dir * Distance);
		const auto EaseSin = [](float x)
		{
			return -(cos(PI * x) - 1) / 2;
		};
		Pos.Z = FMath::Lerp(Actor->GetActorLocation().Z - (60.0f), Actor->GetActorLocation().Z + (60.0f), EaseSin((D.Current * 2.0f) + (index % 2 * 0.5f)));
		//A->SetActorLocation(FMath::Lerp(A->GetActorLocation(), Actor->GetActorLocation(), DeltaTime) + +D.Origin + (Dir * Distance));
		A->GetOwner()->SetActorLocation(Pos);
		//const int BufferIndex = (Heights.Num() - 1) - index;
		//const float Height = BufferIndex >= 0 ? Heights[BufferIndex] : Heights[0];
		//Pos.Z = Height;
		
		//DrawDebugSphere(World, Center.TransformPosition(D.Origin + (Dir * Distance)), 2.0f, 32, FColor::Red, false);
	}
	//FVector::RotateAngleAxis();
	//DrawDebugSphere(World, Actor->GetActorLocation() + FVector{ 0.0f, Width, 0.0f  }, 2.0f, 32, FColor::Red, false);
	//DrawDebugSphere(World, Actor->GetActorLocation() - FVector{ 0.0f, Width, 0.0f  }, 2.0f, 32, FColor::Red, false);
	//DrawDebugSphere(World, Actor->GetActorLocation() + FVector{ Width, 0.0f, 0.0f  }, 2.0f, 32, FColor::Red, false);
	//DrawDebugSphere(World, Actor->GetActorLocation() - FVector{ Width, 0.0f, 0.0f  }, 2.0f, 32, FColor::Red, false);
	//DrawDebugSphere(World, Actor->GetActorLocation() + FVector{ 0.0f, 0.0f, Height }, 2.0f, 32, FColor::Red, false);
	//DrawDebugSphere(World, Actor->GetActorLocation() - FVector{ 0.0f, 0.0f, Height }, 2.0f, 32, FColor::Red, false);
	Heights.Push(Actor->GetActorLocation().Z);
	// ...
}

void UFG_CollectorComponent::Add(UFG_CollectibleComponent* Actor)
{
	CollectedActors.Push(Actor);
	const float Start = FMath::DegreesToRadians(0.0f);
	const float End = FMath::DegreesToRadians(359.9f);
	RotationData.Push(Data
	{ 
		FVector(0.0f, 0.0f, Capsule->GetScaledCapsuleHalfHeight() * 0.25f), 
		FVector(0.0f, 0.0f, 1.0f).GetSafeNormal(), 
		Start, 
		End, 
		FMath::RandRange(0.0f, 1.0f) }
	);
	float Frac = 1.0f / RotationData.Num();
	for (int i = 0; i < RotationData.Num(); i++)
	{
		RotationData[i].Current = Frac * i;
	}
}

void UFG_CollectorComponent::Remove(UFG_CollectibleComponent* Actor)
{
	CollectedActors.Remove(Actor);
	RotationData.Pop();
}

int UFG_CollectorComponent::Num() const
{
	return CollectedActors.Num();
}

UFG_CollectibleComponent* UFG_CollectorComponent::Pop()
{
	RotationData.Pop();
	return CollectedActors.Pop(false);
}

