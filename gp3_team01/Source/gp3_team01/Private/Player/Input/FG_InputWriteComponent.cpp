// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Input/FG_InputWriteComponent.h"
#include "Player/FG_DA_InputStats.h"

// Sets default values for this component's properties
UFG_InputWriteComponent::UFG_InputWriteComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// Tick manually
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UFG_InputWriteComponent::HandleHorizontal(float value)
{
	InputStats->SetRawInputVector(value, InputStats->GetRawInputVector().Y);
}

void UFG_InputWriteComponent::HandleVertical(float value)
{
	InputStats->SetRawInputVector(InputStats->GetRawInputVector().X, value);
}

void UFG_InputWriteComponent::HandleJumpReleased()
{
	InputStats->SetIsJumping(false);
}

void UFG_InputWriteComponent::HandleJumpPressed()
{
	InputStats->SetIsJumping(true);
}

void UFG_InputWriteComponent::HandleGlideReleased()
{
	InputStats->SetIsGliding(false);
}

void UFG_InputWriteComponent::HandleGlidePressed()
{
	InputStats->SetIsGliding(true);
}


// Called when the game starts
void UFG_InputWriteComponent::BeginPlay()
{
	Super::BeginPlay();
	InputStats->InvalidateBuffers();

	InputComponent = GetOwner()->InputComponent;
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &UFG_InputWriteComponent::HandleJumpReleased);
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UFG_InputWriteComponent::HandleJumpPressed);
	InputComponent->BindAction(TEXT("Glide"), IE_Released, this, &UFG_InputWriteComponent::HandleGlideReleased);
	InputComponent->BindAction(TEXT("Glide"), IE_Pressed, this, &UFG_InputWriteComponent::HandleGlidePressed);
	InputComponent->BindAxis(TEXT("Horizontal"), this, &UFG_InputWriteComponent::HandleHorizontal);
	InputComponent->BindAxis(TEXT("Vertical"), this, &UFG_InputWriteComponent::HandleVertical);
	// ...
}


// Called every frame
void UFG_InputWriteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


