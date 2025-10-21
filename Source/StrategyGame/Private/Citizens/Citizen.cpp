// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/Citizen.h"

#include "Citizens/CitizenAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ACitizen::ACitizen()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MannequinMeshFinder(TEXT("/Game/Assets/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"));
	if (MannequinMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MannequinMeshFinder.Object);

		ConstructorHelpers::FClassFinder<UAnimInstance> AnimationBlueprintFinder(TEXT("/Game/Assets/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C"));
		if (AnimationBlueprintFinder.Succeeded())
		{
			GetMesh()->SetAnimInstanceClass(AnimationBlueprintFinder.Class);
		}		
	}

	GetMesh()->SetRelativeLocation(FVector::ZeroVector + FVector::DownVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AIControllerClass = ACitizenAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;
}

// Called when the game starts or when spawned
void ACitizen::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ACitizen::TryGetIsEmployed_Implementation()
{
	return IsEmployed();
}

bool ACitizen::TryGetIsHomeless_Implementation()
{
	return IsHomeless();
}

void ACitizen::SetCitizenType(const ECitizenType NewCitizenType)
{
	CitizenType = NewCitizenType;
}

ECitizenType ACitizen::GetCitizenType() const
{
	return CitizenType;
}

AStructure* ACitizen::GetWorkplace() const
{
	return Workplace;
}

AStructure* ACitizen::GetHome() const
{
	return Home;
}

void ACitizen::AssignWorkplace(AStructure* InWorkplace)
{
	Workplace = InWorkplace;
	OnWorkplaceAssigned.Broadcast(Workplace);
}

void ACitizen::AssignHome(AStructure* InHome)
{
	Home = InHome;
	OnHomeAssigned.Broadcast(Home);
}

bool ACitizen::IsEmployed() const
{
	return Workplace != nullptr;
}

bool ACitizen::IsHomeless() const
{
	return Home == nullptr;
}
