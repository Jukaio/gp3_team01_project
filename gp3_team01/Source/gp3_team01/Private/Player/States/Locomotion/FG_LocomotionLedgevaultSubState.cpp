// Fill out your copyright notice in the Description page of Project Settings.



#include "Player/States/Locomotion/FG_LocomotionLedgevaultSubState.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_PlayerCharacter.h"
#include "Player/FG_SFSM.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/States/Locomotion/FG_LocomotionJumpingSubState.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/FG_DA_LedgevaultStats.h"
#include "Player/ABP_Player.h"
#include "Audio/FG_DA_Instrument.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Audio/InstrumentChord.h"
#include <algorithm>

void UFG_LocomotionLedgevaultSubState::OnStatePush_Implementation()
{
    Super::OnStatePush_Implementation();
    OnStartLedgeVaulting(FGPlayerCharacter);
}

void UFG_LocomotionLedgevaultSubState::OnStatePop_Implementation()
{
    FGPlayerCharacter->LocomotionComp->DisableLedgeGrab();
    OnEndLedgeVaulting(FGPlayerCharacter);
    if (Context->InstrumentHandler != nullptr)
    {
        if (RandomIndeces.Num() == 0)
        {
            for (int i = 0; i < Context->Bells->Num(); i++)
            {
                RandomIndeces.Push(i);
            }
            std::random_shuffle(RandomIndeces.GetData(), RandomIndeces.GetData() + RandomIndeces.Num());
        }
        const int RandomIndex = RandomIndeces.Pop(false);
        Context->InstrumentHandler->PlayChord(Context->Bells->Find(RandomIndex), 0.6f, 1.35f, 0);
    }
}

bool UFG_LocomotionLedgevaultSubState::OnStateTick_Implementation(float DeltaTime)
{
    //Ledgevault is simply a one frame state now that acts as a jump

    Super::OnStateTick_Implementation(DeltaTime);

    UFG_DA_PlayerStats* Stats = FGPlayerCharacter->Stats;
    UFG_LocomotionComponent* Locomotion = FGPlayerCharacter->LocomotionComp;
    GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.5f, FColor::Yellow, TEXT("Ledgevault"));

    Locomotion->DisableFloat(0.5f);
    FVector LedgejumpVector = FGPlayerCharacter->Collider->GetComponentVelocity();
    LedgejumpVector = LedgejumpVector * 0.6f;
    LedgejumpVector.Z = Stats->LedgeStats->LedgevaultJumpForce;
    FGPlayerCharacter->Collider->SetPhysicsLinearVelocity(LedgejumpVector);

    FVector Normal0Z = Locomotion->LedgeNormal;
    Normal0Z.Z = 0;
    Normal0Z.Normalize();

    Locomotion->RotatePlayerRAW(Normal0Z * -1, 30.f);
    //Make this a wee ol' function

    FGPlayerCharacter->GetABP()->Montage_Play(FGPlayerCharacter->LedgejumpMontage, 1, EMontagePlayReturnType::MontageLength, 0, true);

    return false;
    
}
