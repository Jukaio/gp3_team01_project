// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Gliding/FG_GlidingIdleSubState.h"
#include "Player/States/Gliding/FG_GlidingSteeringSubState.h"
#include "Player/ABP_Player.h"
#include "Player/ABP_Glider.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"
#include <algorithm>

// Sets default values for this component's properties
UFG_GlidingPlayerState::UFG_GlidingPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	GlidingIdleSubState = CreateDefaultSubobject<UFG_GlidingIdleSubState>(TEXT("GlidingIdleState"));
	GlidingSteeringSubState = CreateDefaultSubobject<UFG_GlidingSteeringSubState>(TEXT("GlidingSteeringState"));
	// ...
}


// Called when the game starts
void UFG_GlidingPlayerState::BeginPlay()
{
	Super::BeginPlay();
	GliderComponent = FGPlayerCharacter->Glider;
	FGPlayerCharacter->Collider->OnComponentHit.AddDynamic(this, &UFG_GlidingPlayerState::OnCollision);
	UE_LOG(LogTemp, Warning, TEXT("Start"));
	SubStateMachine = NewObject<UFG_SFSM>();


	InstrumentHandler = Cast<UFG_InstrumentComponent>(GetOwner()->GetComponentByClass(UFG_InstrumentComponent::StaticClass()));
}


// Called every frame
void UFG_GlidingPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFG_GlidingPlayerState::OnStatePush_Implementation()
{
	Super::OnStatePush_Implementation();
	SubStateMachine->Push(GlidingIdleSubState);
	GliderComponent->StartGliding();
	
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	CapsuleCollider->SetEnableGravity(false);
	FGPlayerCharacter->GetABP()->bIsGliding = true;
	FGPlayerCharacter->GetGliderABP()->bIsActive = true;
	FGPlayerCharacter->GetGliderABP()->Montage_Play(FGPlayerCharacter->GliderEquipMontage, 1, EMontagePlayReturnType::MontageLength, 0, true);
	FGPlayerCharacter->GliderMesh->SetVisibility(true, false);
	bIsColliding = false;
}

void UFG_GlidingPlayerState::OnStatePop_Implementation()
{
	Super::OnStatePop_Implementation();
	SubStateMachine->Clear();
	GliderComponent->StopGliding();
	UCapsuleComponent* CapsuleCollider = FGPlayerCharacter->Collider;
	FGPlayerCharacter->GetABP()->bIsGliding = false;
	FGPlayerCharacter->GetGliderABP()->bIsActive = false;
	FGPlayerCharacter->GliderMesh->SetVisibility(false, false);
	CapsuleCollider->SetEnableGravity(true);
}

bool UFG_GlidingPlayerState::OnStateTick_Implementation(float DeltaTime)
{
	GliderComponent->CheckTouching(FGPlayerCharacter->Collider);
	SubStateMachine->OnStateMachineTick(DeltaTime);
	
	GliderComponent->Glide();
	const bool IsGlidingPressed = FGPlayerCharacter->Stats->InputStats->GetIsGlidingPressed();
	
	if(bIsColliding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exited glide state"));
		return false;
	}

	const float WorldTime = GetWorld()->TimeSeconds;
	const float Difference = WorldTime - LastTimeMoveWasPlayed;
	if (ensure(InstrumentHandler != nullptr) && Difference > PlayTimeBuffer)
	{
		if (RandomIndeces.Num() == 0)
		{
			for (int i = 0; i < Chimes->Num(); i++)
			{
				RandomIndeces.Push(i);
			}
			std::random_shuffle(RandomIndeces.GetData(), RandomIndeces.GetData() + RandomIndeces.Num());
		}
		const int RandomIndex = RandomIndeces.Pop(false);
		const UInstrumentChord* Chord = Chimes->Find(RandomIndex);
		PlayTimeBuffer = Chord->Notes[0]->Duration * 0.9f;
		InstrumentHandler->PlayChord(Chord, 0.45f, 1.0f, 0, 0.15f, 0.9f);
		LastTimeMoveWasPlayed = WorldTime;
	}
	
	const bool IsLanding = GliderComponent->CheckTouchingGround(FGPlayerCharacter->Stats->HoverStats->HoverRayDistance);
	return IsGlidingPressed && !IsLanding;
	//UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
}

void UFG_GlidingPlayerState::OnCollision(UPrimitiveComponent* PrimitiveComponent, AActor* Actor,
	UPrimitiveComponent* PrimitiveComponent1, FVector Vector, const FHitResult& HitResult)
{
	bIsColliding = true;
	UE_LOG(LogTemp, Warning, TEXT("Start collision"));
}

