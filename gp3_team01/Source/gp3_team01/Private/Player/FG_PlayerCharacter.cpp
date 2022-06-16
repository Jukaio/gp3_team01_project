// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/FG_PlayerCharacter.h"
#include "Player/Movement/FG_LocomotionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/Input/FG_InputWriteComponent.h"
#include "Player/Gliding/FG_GliderComponent.h"
#include "Player/States/Core/FG_LocomotionPlayerState.h"
#include "Player/States/Core/FG_GlidingPlayerState.h"
#include "Player/ABP_Player.h"
#include "Player/ABP_Glider.h"
#include "Player/FG_DA_PlayerStats.h"
#include "Player/FG_DA_InputStats.h"
#include "Audio/FG_InstrumentComponent.h"
#include "Cameras/FG_Camera.h"
#include "Player/FG_SFSM.h"
#include "TemplateHelper.h"


// Sets default values
AFG_PlayerCharacter::AFG_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collider"));
	RootComponent = Collider;
	InputWriter = CreateDefaultSubobject<UFG_InputWriteComponent>(TEXT("InputWriter"));
	LocomotionComp = CreateDefaultSubobject<UFG_LocomotionComponent>(TEXT("LocomotionComponent"));
	InstrumentUser = CreateDefaultSubobject<UFG_InstrumentComponent>(TEXT("InstrumentComponent"));
	Glider = CreateDefaultSubobject<UFG_GliderComponent>(TEXT("Glider"));

	MeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPivot"));
	MeshPivot->SetupAttachment(Collider);

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMeshComponent"));
	PlayerMesh->SetupAttachment(MeshPivot);

	GliderMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GliderMeshComponent"));
	GliderMesh->SetupAttachment(MeshPivot);
	
}

// Called when the game starts or when spawned
void AFG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	const UFG_DA_PlayerStateClasses* StateClasses = Stats->PlayerStateClasses;
	LocomotionCoreState = CreateAndAttachSubState(this, StateClasses->LocomotionPlayerState);
	GlidingCoreState = CreateAndAttachSubState(this, StateClasses->GlidingPlayerState);

	FTransform CamSpawnTransform = GetActorTransform();
	MyCamera = Cast<AFG_Camera>(GetWorld()->SpawnActor(CameraClass, &CamSpawnTransform));

	StateMachine = NewObject<UFG_SFSM>();
	StateMachine->Push(LocomotionCoreState);

	AnimBP = Cast<UABP_Player>(PlayerMesh->GetAnimInstance());
	GliderAnimBP = Cast<UABP_Glider>(GliderMesh->GetAnimInstance());
	GliderMesh->SetVisibility(false, false);
}

// Called every frame
void AFG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StateMachine->OnStateMachineTick(DeltaTime);

	Stats->InputStats->UpdateBuffers();

	//Sets the ground velocity data in the animinstance
	FVector Velocity0Z = Collider->GetPhysicsLinearVelocity();
	Velocity0Z.Z = 0;
	GetABP()->VelocityDist = Velocity0Z.Size();
	
	//Sets the vertical velocity in the animinstance
	GetABP()->VerticalVelocity = Collider->GetPhysicsLinearVelocity().Z;

	GetABP()->bIsGrounded = LocomotionComp->bGrounded;

	GetGliderABP()->VelocityDist = Collider->GetPhysicsLinearVelocity().Size();
}

AFG_Camera* AFG_PlayerCharacter::GetMyCamera()
{
	return MyCamera;
}

UABP_Player* AFG_PlayerCharacter::GetABP()
{
	
	return AnimBP;
}

UABP_Glider* AFG_PlayerCharacter::GetGliderABP()
{
	return GliderAnimBP;
}


