// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FG_PlayerCharacter.generated.h"

class AFG_Camera;
class UABP_Player;
UCLASS()
class GP3_TEAM01_API AFG_PlayerCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFG_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// <summary>
	/// Returns a reference to your FG_Camera
	/// </summary>
	/// <returns></returns>
	AFG_Camera* GetMyCamera();

	/// <summary>
	/// Returns a reference to the AnimInstance
	/// </summary>
	UABP_Player* GetABP();

	class UABP_Glider* GetGliderABP();


public: //Components

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_InputWriteComponent* InputWriter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_LocomotionComponent* LocomotionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_GliderComponent* Glider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_CollectorComponent* Collector;

	UPROPERTY(BlueprintReadOnly)
	class UFG_SFSM* StateMachine;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* Collider;

	//Attach the visual mesh onto this!
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* MeshPivot;

	UPROPERTY(VisibleAnywhere, Category = Animations)
	class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(VisibleAnywhere, Category = Animations)
	class USkeletalMeshComponent* GliderMesh;

	

	

public: //Public Variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_DA_PlayerStats* Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_LocomotionPlayerState* LocomotionCoreState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_GlidingPlayerState* GlidingCoreState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UFG_InstrumentComponent* InstrumentUser;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFG_Camera> CameraClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	class UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* LandMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* LedgejumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* GliderEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
	UAnimMontage* GliderUnequippMontage;

	


	

private: //Private Variables

	UPROPERTY(EditDefaultsOnly)
	AFG_Camera* MyCamera;

	UPROPERTY()
	UABP_Player* AnimBP; //Cast the playermeshes AnimClass into an UABP_Player in BeginPlay and work the magic from there boyo

	UPROPERTY()
	UABP_Glider* GliderAnimBP;
	


};
