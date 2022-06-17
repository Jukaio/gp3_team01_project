// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/Collectible/FG_CollectibleUnlocker.h"
#include "LevelElements/Collectible/FG_CollectorComponent.h"
#include "LevelElements/Collectible/FG_CollectibleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"

// Sets default values for this component's properties
UFG_CollectibleUnlocker::UFG_CollectibleUnlocker()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//UStaticMeshComponent
	// ...
}


// Called when the game starts
void UFG_CollectibleUnlocker::BeginPlay()
{
	UActorComponent::BeginPlay();
	//MeshComponents = GetComponentsByClass(UMeshComponent::StaticClass());
	// ...
	GetOwner()->GetComponents(MeshComponents);
	for (auto& Mesh : MeshComponents)
	{
		Mesh->SetMaterial(0, LockedMaterial);
	}
}


// Called every frame
void UFG_CollectibleUnlocker::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UActorComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_CollectibleUnlocker::OnLocomotionTouch_Implementation(UFG_LocomotionComponent* LocomotionComponentOfCaller, EFG_TouchingState State)
{
	if (!GetOwner()->GetActorEnableCollision())
	{
		// Ignore
		return;
	}

	const AActor* Actor = LocomotionComponentOfCaller->GetOwner();
	UFG_CollectorComponent* Collector = Cast<UFG_CollectorComponent>(Actor->GetComponentByClass(UFG_CollectorComponent::StaticClass()));
	if (State == EFG_TouchingState::StartedTouching)
	{
		if (CurrentCollector == nullptr)
		{
			CurrentCollector = Collector;
		}
		else if (CurrentCollector != Collector)
		{
			// ignore - First come, first serve
			return;
		}
		const UWorld* World = GetWorld();
		const float WorldTime = World->TimeSeconds;
		Timer = 0.0f;
	}
	else if (State == EFG_TouchingState::StillTouching)
	{
		if (CurrentCollector != Collector)
		{
			return;
		}
		const auto EaseSin = [](float x)
		{
			return -(cos(PI * x) - 1) / 2;
		};


		if (TravellingCollectible == nullptr)
		{
			TravellingCollectible = CurrentCollector->Num() > 0 ? CurrentCollector->Pop() : nullptr;
			if (TravellingCollectible != nullptr)
			{
				CollectibleFromPos = TravellingCollectible->GetOwner()->GetActorLocation();
				CollectibleFromRot = TravellingCollectible->GetOwner()->GetActorQuat();
				CollectibleFromScale = TravellingCollectible->GetOwner()->GetActorScale3D();
			}
		}
		if (TravellingMeshComponent == nullptr)
		{
			TravellingMeshComponent = MeshComponents.Num() > 0 ? MeshComponents.Pop() : nullptr;
		}

		if (TravellingCollectible != nullptr && TravellingMeshComponent != nullptr)
		{
			// Main routine
			const UWorld* World = GetWorld();
			const float WorldDeltaTime = World->DeltaTimeSeconds;
			Timer += WorldDeltaTime / FullCycleDuration;

			TravellingCollectible->GetOwner()->SetActorLocation(FMath::Lerp(CollectibleFromPos, TravellingMeshComponent->GetComponentLocation(), EaseSin(Timer)));
			TravellingCollectible->GetOwner()->SetActorRotation(FQuat::Slerp(CollectibleFromRot, TravellingMeshComponent->GetComponentQuat(), EaseSin(Timer)));
			TravellingCollectible->GetOwner()->SetActorScale3D(FMath::Lerp(CollectibleFromScale, TravellingMeshComponent->GetComponentScale(), EaseSin(Timer)));

			Timer += WorldDeltaTime / FullCycleDuration;
			if (Timer > 1.0f)
			{
				UsedMeshComponents.Push(TravellingMeshComponent);
				UsedCollectibles.Push(TravellingCollectible);
				TravellingMeshComponent->SetMaterial(0, UnlockedMaterial);
				TravellingCollectible = nullptr;
				TravellingMeshComponent = nullptr;
				Timer = 0.0f;
			}
		}
		

		//if (index < FMath::Min(MeshComponents.Num(), CurrentCollector->Num()))
		//{
		//	

		//	if (index >= ConsumedCollectibles.Num())
		//	{
		//		const FVector TargetLocation = MeshComponents[index]->GetComponentLocation();
		//		UFG_CollectibleComponent* Collectible = CurrentCollector->Pop();
		//		ConsumedCollectibles.Push( { Collectible, Collectible->GetOwner()->GetActorLocation(), TargetLocation });
		//	}
		//	const FromToCollectible& Current = ConsumedCollectibles[index];
		//	const float TimeDifference = End - Start;
		//	const float TimeDifferenceAsFracton = TimeDifference / FullCycleDuration;
		//	Current.Context->GetOwner()->SetActorLocation(FMath::Lerp(Current.From, Current.To, TimeDifferenceAsFracton));
		//	if (MeshComponents[index]->GetMaterial(0) != UnlockedMaterial)
		//	{
		//		MeshComponents[index]->SetMaterial(0, UnlockedMaterial);
		//	}
		//}
	}
	else if (State == EFG_TouchingState::StoppedTouching)
	{
		if (CurrentCollector == nullptr)
		{
			return;
		}
		else if (CurrentCollector != Collector)
		{
			return;
		}

		if (TravellingCollectible != nullptr)
		{
			CurrentCollector->Add(TravellingCollectible);
			TravellingCollectible = nullptr;
		}
		if (TravellingMeshComponent != nullptr)
		{
			MeshComponents.Push(TravellingMeshComponent);
			TravellingMeshComponent = nullptr;
		}

		for (auto& Mesh : MeshComponents)
		{
			Mesh->SetMaterial(0, LockedMaterial);
		}
		CurrentCollector = nullptr;
	}
}

