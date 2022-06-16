// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_SFSM.h"
#include "TemplateHelper.h"
#include "Player/States/Locomotion/FG_LocomotionIdleSubState.h"
#include "Player/States/Locomotion/FG_LocomotionMovingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirborneSubState.h"
#include "Player/States/Locomotion/FG_LocomotionAirJumpSubState.h"
#include "Player/States/Locomotion/FG_LocomotionLedgevaultSubState.h"
#include "Player/States/Locomotion/FG_LocomotionWallclimbSubState.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"

// Sets default values for this component's properties
UFG_LocomotionPlayerState::UFG_LocomotionPlayerState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UFG_LocomotionPlayerState::BeginPlay()
{
	Super::BeginPlay();
	const UFG_DA_PlayerStateClasses* StateClasses = GetFGPlayer()->Stats->PlayerStateClasses;
	LocomotionJumpingSubState = CreateAndAttachSubState(this, StateClasses->LocomotionJumpSubStateClass);
	LocomotionIdleSubState = CreateAndAttachSubState(this, StateClasses->LocomotionIdleSubStateClass);
	LocomotionMovingSubState = CreateAndAttachSubState(this, StateClasses->LocomotionMovingSubStateClass);
	LocomotionAirborneSubState = CreateAndAttachSubState(this, StateClasses->LocomotionAirborneSubStateClass);
	LocomotionLedgevaultSubState = CreateAndAttachSubState(this, StateClasses->LocomotionLedgevaultSubStateClass);
	LocomotionWallclimbSubState = CreateAndAttachSubState(this, StateClasses->LocomotionWallclimbSubStateClass);
	LocomotionAirJumpSubState = CreateAndAttachSubState(this, StateClasses->LocomotionAirJumpSubStateClass);

	SubStateMachine = NewObject<UFG_SFSM>();
	SubStateMachine->Push(LocomotionIdleSubState);

	CurrentChord = Instrument->Find(0);
	CurrentFallChord = CurrentChord->Next(1);

	InstrumentHandler = Cast<UFG_InstrumentComponent>(GetOwner()->GetComponentByClass(UFG_InstrumentComponent::StaticClass()));
	//JumpBuffer = -FGPlayerCharacter->Stats->JumpStats->JumpBufferThreshold;
	// ...
}


// Called every frame
void UFG_LocomotionPlayerState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UFG_LocomotionPlayerState::OnStateTick_Implementation(float DeltaTime)
{
	Super::OnStateTick_Implementation(DeltaTime);

	if (FGPlayerCharacter == nullptr)
	{
		// Print Error here
		return false;
	}
	UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
	if (Locomotion == nullptr)
	{
		// Print Error here
		return false;
	}
	UPrimitiveComponent* Primitive = FGPlayerCharacter->Collider;
	if (Primitive == nullptr)
	{
		// Print Error here
		return false;
	}
	UFG_DA_PlayerStats* Stats = FGPlayerCharacter->Stats;
	if (Stats == nullptr)
	{
		// Print Error here
		return false;
	}

	BufferJumpState();

	Locomotion->ApplyGravity(Stats->JumpStats->Gravity);
	Locomotion->CheckTouching(Primitive);

	CheckAndSetGrounded(Locomotion, Stats->HoverStats);

	SubStateMachine->OnStateMachineTick(DeltaTime);
	return true;
}

bool UFG_LocomotionPlayerState::IsGroundedWithResult(TArray<FHitResult>& OutHit) const
{
	OutHit = GroundHit;
	return bIsGrounded;
}

bool UFG_LocomotionPlayerState::IsGrounded() const
{
	return bIsGrounded;
}

bool UFG_LocomotionPlayerState::IsCoyoteGrounded() const
{
	const float CoyoteTime = FGPlayerCharacter->Stats->JumpStats->CoyoteTime;
	const bool CanStillJump = !(GetTimeSincePush() > CoyoteTimeStart + CoyoteTime);
	return IsGrounded() || CanStillJump;
}

bool UFG_LocomotionPlayerState::ShouldStartJump()
{
	if (FGPlayerCharacter->Stats->InputStats->GetWasJumpingJustPressed()) {
		return true;
	}
	if (JumpBuffer.Num() == 0)
	{
		return false;
	}

	for (uint64 i = 0; i < JumpBuffer.Num(); i++)
	{
		const uint64 index = (JumpBuffer.Num() - i) - 1;
		TimedInput CurrentJumpInput = JumpBuffer[index];

		const float WorldTimeSecond = GetWorld()->TimeSeconds;
		const float TimeDifference = WorldTimeSecond - CurrentJumpInput.TimePoint;
		const bool IsJumping = CurrentJumpInput.State && TimeDifference <= FGPlayerCharacter->Stats->JumpStats->JumpBufferThreshold;
		if (IsJumping)
		{
			return true;
		}
	}
	return false;
}

void UFG_LocomotionPlayerState::BufferJumpState()
{
	const UFG_DA_InputStats* InputStats = FGPlayerCharacter->Stats->InputStats;
	const float WorldTimeSecond = GetWorld()->TimeSeconds;
	const float JumpBufferThreshold = FGPlayerCharacter->Stats->JumpStats->JumpBufferThreshold;
	
	if (SubStateMachine->TopIsEqual(LocomotionJumpingSubState)) {
		CurrentFallChord = CurrentChord->Next(1);
		JumpBuffer.Reset();
	}
	JumpBuffer.RemoveAll([WorldTimeSecond, JumpBufferThreshold](TimedInput& item) {
		const float TimeDifference = WorldTimeSecond - item.TimePoint;
		const bool IsValid = TimeDifference > JumpBufferThreshold;
		return IsValid;
	});

	if (InputStats->GetWasJumpingJustPressed()) 
	{
		JumpBuffer.Add({ WorldTimeSecond, true });
	}
	if (InputStats->GetWasJumpingJustReleased()) 
	{
		JumpBuffer.Add({ WorldTimeSecond, false });
	}
}

void UFG_LocomotionPlayerState::CheckAndSetGrounded(UFG_LocomotionComponent* Locomotion, UFG_DA_HoverStats* HoverStats)
{
	bWasGrounded = bIsGrounded;
	const bool bWasCoyoteGrounded = IsCoyoteGrounded();
	bIsGrounded = Locomotion->Hover(HoverStats, GroundHit);
	if (bWasGrounded && !IsGrounded())
	{
		CoyoteTimeStart = GetTimeSincePush();
	}

	if (!bWasCoyoteGrounded && IsCoyoteGrounded())
	{
		PlayerLand();
	}
}

void UFG_LocomotionPlayerState::PlayerLand()
{
	OnPlayerLand(FGPlayerCharacter);
	if (InstrumentHandler != nullptr)
	{
		InstrumentHandler->PlayChord(CurrentFallChord, 0.6f);
		CurrentFallChord = CurrentFallChord->Prev(1);
	}
}
