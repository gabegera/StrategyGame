// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/Citizen.h"

#include "Building/Structure.h"
#include "Citizens/CitizenAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/HousingComponent.h"
#include "Components/WorkersComponent.h"
#include "Game/StrategyGameInstance.h"
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

	UStrategyGameInstance* GameInstance = GetGameInstance<UStrategyGameInstance>();
	GameInstance->OnStructureBuilt.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
	GameInstance->OnStructureDestroyed.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);
	GameInstance->OnResidentRequested.AddUniqueDynamic(this, &ThisClass::OnResidentRequested);
	GameInstance->OnWorkerRequested.AddUniqueDynamic(this, &ThisClass::OnWorkerRequested);
	
}

void ACitizen::OnStructureBuilt(AStructure* NewStructure)
{

}

void ACitizen::OnStructureDestroyed(AStructure* DestroyedStructure)
{
	if (Home == DestroyedStructure)
	{
		ClearHome();
	}

	if (Workplace == DestroyedStructure)
	{
		ClearWorkplace();
	}
}

void ACitizen::OnResidentRequested(AStructure* StructureSource, UHousingComponent* Housing)
{
	if (IsHomeless())
    {
    	if (Housing && !Housing->IsFullCapacity())
    	{
    		AssignHome(StructureSource);
    		Housing->AssignResident(this);
    	}
    }
}

void ACitizen::OnWorkerRequested(AStructure* StructureSource, UWorkersComponent* Workers, const ECitizenType RequestedCitizenType)
{
    if (Workers && CitizenType == RequestedCitizenType && !Workers->IsFullCapacity())
    {
    	AssignWorkplace(StructureSource);
    	Workers->AssignWorker(this);
    }
}

ECitizenType ACitizen::TryGetCitizenType_Implementation()
{
	return GetCitizenType();
}

ECitizenState ACitizen::TrySetCitizenState_Implementation(const ECitizenState InCitizenState)
{
	return SetCitizenState(InCitizenState);
}

ECitizenState ACitizen::TryGetCitizenState_Implementation()
{
	return GetCitizenState();
}

AStructure* ACitizen::TryGetHome_Implementation()
{
	return GetHome();
}

AStructure* ACitizen::TryGetWorkplace_Implementation()
{
	return GetWorkplace();
}

bool ACitizen::TryGetIsEmployed_Implementation()
{
	return IsEmployed();
}

bool ACitizen::TryGetIsHomeless_Implementation()
{
	return IsHomeless();
}

void ACitizen::TryToEnterStructure_Implementation(AStructure* TargetStructure)
{
	EnterStructure(TargetStructure);
}

void ACitizen::TryToExitStructure_Implementation()
{
	ExitStructure();
}

void ACitizen::ClearHome()
{
	Home = nullptr;
	OnHomeCleared.Broadcast();
}

void ACitizen::ClearWorkplace()
{
	Workplace = nullptr;
	OnWorkplaceCleared.Broadcast();
}

void ACitizen::EnterStructure(AStructure* TargetStructure)
{
	if (!TargetStructure) return;

	SetActorHiddenInGame(true);
	GetCharacterMovement()->GravityScale = 0.0f;
	SetActorEnableCollision(false);
	SetActorLocation(TargetStructure->GetActorLocation() + FVector::UpVector * GetCapsuleComponent()->GetLocalBounds().BoxExtent.Z);

}

void ACitizen::ExitStructure()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	GetCharacterMovement()->GravityScale = 1.0f;
}

ECitizenState ACitizen::SetCitizenState(const ECitizenState InCitizenState)
{
	return CitizenState = InCitizenState;
}

ECitizenState ACitizen::GetCitizenState() const
{
	return CitizenState;
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
