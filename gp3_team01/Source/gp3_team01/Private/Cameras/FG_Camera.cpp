// Fill out your copyright notice in the Description page of Project Settings.


#include "Cameras/FG_Camera.h"
#include "Camera/CameraComponent.h"
#include "Cameras/FG_DA_CameraStats.h"
#include "Player/FG_DA_InputStats.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFG_Camera::AFG_Camera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Root);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);



}

// Called when the game starts or when spawned
void AFG_Camera::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	Controller->SetViewTarget(this); 
	Controller->InputComponent->BindAxis(TEXT("MouseX"), this, &AFG_Camera::HandleCameraXRotation);
	Controller->InputComponent->BindAxis(TEXT("MouseY"), this, &AFG_Camera::HandleCameraYRotation);

	FollowActor = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

// Called every frame
void AFG_Camera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ReturnVector = Camera->GetRightVector() * InputStats->GetRawInputVector().X + Camera->GetForwardVector() * InputStats->GetRawInputVector().Y;
	ReturnVector.Z = 0;
	ReturnVector.Normalize();
	InputStats->SetCameraNormalisedInputVector(ReturnVector.X, ReturnVector.Y);
	InputStats->SetCameraForwardVector(Camera->GetForwardVector());


	if (FollowActor == nullptr) 
	{
		return;
	}
	FVector Location = GetActorLocation();
	Location = FMath::Lerp(Location, FollowActor->GetActorLocation(), CameraStats->CameraFollowSpeed * DeltaTime); 
	//This is one of these infinitely closing lerps I think. Technically broken but works good enough.

	SetActorLocation(Location);

}

void AFG_Camera::SetFollowActor(AActor* Actor)
{
	FollowActor = Actor;
}

void AFG_Camera::SetManuallyControllable(bool bSetManualControl)
{
	bManuallyControllable = bSetManualControl;
}

void AFG_Camera::RotateCamera(FVector Direction, float RotationDelta)
{
	//ToOrientationQuat does not like 0 vectors

	//Fuck shit this doesnt work

	if (Direction == FVector::ZeroVector) 
	{
		return;
	}
	Direction.Normalize();

	FRotator TargetRotation = Direction.ToOrientationRotator();
	float YAxis = Camera->GetRelativeRotation().Pitch;
	YAxis = FMath::Lerp(YAxis, TargetRotation.Pitch, RotationDelta);

	float XAxis = SpringArm->GetRelativeRotation().Yaw;
	XAxis = FMath::Lerp(XAxis, TargetRotation.Yaw, RotationDelta);
	
	FRotator CameraRotator = FRotator(YAxis, 0, 0);
	FRotator SpringArmRotator = FRotator(0, XAxis, 0);

	Camera->SetWorldRotation(CameraRotator);
	SpringArm->SetWorldRotation(SpringArmRotator);
}

//Note that the XRotation and YRotation functions rotates 2 seperate SceneComponents on their respective axises, since SpringArm is parented on the root component 
//Alfons isn't good at handling rotation math.
void AFG_Camera::HandleCameraXRotation(float InputAxis)
{
	if (!bManuallyControllable)
	{
		return;
	}
	AddActorLocalRotation(FRotator(0, CameraStats->CameraPanSpeed * InputAxis, 0));
	
}


void AFG_Camera::HandleCameraYRotation(float InputAxis)
{
	if (!bManuallyControllable)
	{
		return;
	}
	float YAxis = SpringArm->GetTargetRotation().Pitch + CameraStats->CameraPanSpeed * InputAxis;
	YAxis = FMath::Clamp(YAxis, CameraStats->MinimumTurnAngle, CameraStats->MaxTurnAngle);

	FRotator SpringArmRotation = SpringArm->GetTargetRotation();
	SpringArmRotation.Pitch = YAxis;
	SpringArm->SetWorldRotation(SpringArmRotation);
}

