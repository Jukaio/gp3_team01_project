// Fill out your copyright notice in the Description page of Project Settings.
#include "LevelElements/Winddraft/FG_Winddraft.h"
#include "Player/FG_PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Components/BoxComponent.h"

// Sets default values
AFG_Winddraft::AFG_Winddraft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxTrigger->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFG_Winddraft::BeginPlay()
{
	Super::BeginPlay();

}

void AFG_Winddraft::ApplyWindForce(UPrimitiveComponent* ActorCapsuleComponent, AFG_PlayerCharacter* Player)
{
	UFG_GliderComponent* GliderComponent = Cast<UFG_GliderComponent>(Player->GetComponentByClass(UFG_GliderComponent::StaticClass()));
	GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Wind"));

	if (GliderComponent->bIsGliding)
	{
		ActorCapsuleComponent->AddForce(GetActorUpVector() * WindSpeedGliding);
		//CapsuleComponent->AddImpulse(GetActorUpVector() * WindSpeedGliding);
	}
	else
	{
		ActorCapsuleComponent->AddForce(GetActorUpVector() * WindSpeedNotGliding);
		//CapsuleComponent->AddImpulse(GetActorUpVector() * WindSpeedNotGliding);
	}

}

