// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FG_SFSM.generated.h"

class IFG_State;

USTRUCT()
struct FStateObjectPair
{
	GENERATED_BODY()

	UObject* Object = nullptr;
	IFG_State* State = nullptr;
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_TEAM01_API UFG_SFSM : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UFG_SFSM();

protected:

public:
	// Called every frame
	UFUNCTION(BlueprintCallable)
	void OnStateMachineTick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void Push(UPARAM(ref) const TScriptInterface<IFG_State>& State);

	UFUNCTION(BlueprintCallable)
	void Pop();

	UFUNCTION(BlueprintPure)
	bool Contains(UPARAM(ref) const TScriptInterface<IFG_State>& State) const;

	UFUNCTION(BlueprintCallable)
	bool PopUntil(UPARAM(ref) TScriptInterface<IFG_State>& State);

	UFUNCTION(BlueprintCallable)
	void Clear();

	UFUNCTION(BlueprintPure)
	bool IsEmpty();

private:

	UPROPERTY()
	TArray<FStateObjectPair> StateStack;
	UPROPERTY()
	bool bIsPoppingNotAllowed = false;
};
