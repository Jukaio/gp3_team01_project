// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FG_SFSM.h"
#include "Player/FG_State.h"

// Sets default values for this component's properties
UFG_SFSM::UFG_SFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}

// Called every frame
void UFG_SFSM::OnStateMachineTick(float DeltaTime)
{
	FString Name = StateStack.Last().Object->GetFName().ToString();
	GEngine->AddOnScreenDebugMessage
	(
		INDEX_NONE,
		DeltaTime,
		FColor::Magenta,
		Name
	);

	const bool StackHasElements = StateStack.Num() != 0;
	if (StackHasElements)
	{
		// Make copy, cause when state gets popped, reference is saying bye bye : )
		bIsPoppingNotAllowed = true;
		FStateObjectPair Current = StateStack.Last();
		bool StackStillHasElements = StateStack.Num() != 0;
		while (StackStillHasElements && Current.State->Execute_OnStateTick(Current.Object, DeltaTime) == false)
		{
			bIsPoppingNotAllowed = false;
			this->Pop(); // Pop latest element on stack

			StackStillHasElements = StateStack.Num() != 0;
			if (StackStillHasElements)
			{
				Current = StateStack.Last();
				Current.State->Execute_OnStateEnter(Current.Object);
			}
			bIsPoppingNotAllowed = true;
		}
		bIsPoppingNotAllowed = false;
	}
	// ...
}

void UFG_SFSM::Pop()
{
	if (bIsPoppingNotAllowed == true) {
		GEngine->AddOnScreenDebugMessage
		(
			INDEX_NONE,
			2.0f,
			FColor::Red,
			TEXT("ERROR: CANNOT POP IN STATE TICK!")
		);
		return;
	}
	if (StateStack.Num() != 0)
	{
		FStateObjectPair State = StateStack.Last();
		State.State->Execute_OnStateExit(State.Object);
		State.State->Execute_OnStatePop(State.Object);
		StateStack.Pop();

		if (StateStack.Num() != 0)
		{
			State = StateStack.Last();
			State.State->Execute_OnStateEnter(State.Object);
		}
	}
}

bool UFG_SFSM::Contains(UPARAM(ref) const TScriptInterface<IFG_State>& State) const
{
	const auto Predicate = [&State](FStateObjectPair data) { return data.Object == State.GetObject(); };
	return StateStack.ContainsByPredicate(Predicate);
}

bool UFG_SFSM::PopUntil(UPARAM(ref) TScriptInterface<IFG_State>& State)
{
	const auto IsSame = [](const TScriptInterface<IFG_State>& lhs, FStateObjectPair rhs) { return lhs.GetObject() == rhs.Object; };
	const bool HasState = Contains(State);
	FStateObjectPair Current = StateStack.Last();
	while (HasState && !IsSame(State, Current)) {
		this->Pop();
		Current = StateStack.Last();
	}
	return HasState;
}

// Simulate a full on popping
void UFG_SFSM::Clear()
{ 
	if (bIsPoppingNotAllowed == true) {
		GEngine->AddOnScreenDebugMessage
		(
			INDEX_NONE,
			2.0f,
			FColor::Red,
			TEXT("ERROR: CANNOT CLEAR IN STATE TICK!")
		);
		return;
	} 
	while (StateStack.Num() != 0)
	{
		FStateObjectPair State = StateStack.Last();
		State.State->Execute_OnStatePop(State.Object);
		State.State->Execute_OnStateExit(State.Object);
		StateStack.Pop();

		if (StateStack.Num() != 0)
		{
			State = StateStack.Last();
			State.State->Execute_OnStateEnter(State.Object);
		}
	}
}

bool UFG_SFSM::IsEmpty()
{
	return StateStack.Num() == 0;
}

void UFG_SFSM::Push(const TScriptInterface<IFG_State>& State)
{
	UObject* Object = State.GetObject();
	const bool IsImplementingInterface = Object->GetClass()->ImplementsInterface(IFG_State::UClassType::StaticClass());
	if (IsImplementingInterface)
	{
		IFG_State* Base = Cast<IFG_State>(Object);
		StateStack.Push({ Object, Base });
		Base->Execute_OnStatePush(Object);
		Base->Execute_OnStateEnter(Object);
	}
}

