// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelElements/BouncePad/FG_BouncePad.h"
#include "Components/BoxComponent.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFG_BouncePad::AFG_BouncePad()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxTrigger->SetupAttachment(Root);

}

void AFG_BouncePad::Bounce(UPrimitiveComponent* ActorCapsuleComponent)
{
	ActorCapsuleComponent->AddImpulse(GetActorUpVector() * BounceForce);
}

