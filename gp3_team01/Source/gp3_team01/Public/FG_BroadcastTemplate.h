// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Runtime/Core/Public/Templates/UnrealTemplate.h>

template<typename Interface, typename ExecuteFunctor, typename... Parameters>
constexpr inline void BroadcastResponse(AActor* Actor, ExecuteFunctor Func, Parameters&&... Params)
{
	if (Actor == nullptr) {
		return;
	}
	const bool IsImplemented = Actor->GetClass()->ImplementsInterface(Interface::UClassType::StaticClass());
	if (IsImplemented)
	{
		Func(Actor, (Params)...);
	}

	const TArray<UActorComponent*> Collection = Actor->GetComponentsByInterface(Interface::UClassType::StaticClass());
	for (auto& Element : Collection)
	{
		Func(Element, (Params)...);
	}
}

template<typename Interface, typename ExecuteFunctor, typename... Parameters>
constexpr inline void BroadcastResponse(const FHitResult& Hit, ExecuteFunctor Func, Parameters&&... Params)
{
	BroadcastResponse<Interface>(Hit.GetActor(), Func, Forward<Parameters>(Params)...);
}