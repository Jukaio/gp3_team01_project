// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

template<typename T>
constexpr inline T* CreateAndAttachSubState(UObject* Owner, TSubclassOf<T> SubClass)
{
	T* Temp = nullptr;
	if (SubClass != nullptr) {
		Temp = NewObject<T>(Owner, SubClass);
	}
	else {
		Temp = NewObject<T>(Owner);
	}
	Temp->RegisterComponent();
	return Temp;
}