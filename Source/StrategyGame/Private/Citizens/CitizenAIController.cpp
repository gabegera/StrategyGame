// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/CitizenAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"


// Sets default values
ACitizenAIController::ACitizenAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/Blueprints/AI/Citizens/BT_Citizen.BT_Citizen"));
	if (BehaviorTreeFinder.Succeeded())
	{
		BehaviorTree = BehaviorTreeFinder.Object;
	}
}

// Called when the game starts or when spawned
void ACitizenAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACitizenAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ACitizen* PossessedCitizen = Cast<ACitizen>(InPawn);
	if (PossessedCitizen && BehaviorTree)
	{
		UseBlackboard(BehaviorTree->GetBlackboardAsset(), BlackboardComponent);
		RunBehaviorTree(BehaviorTree);

		if (PossessedCitizen->IsEmployed())
		{
			BlackboardComponent->SetValueAsVector(BBKey_WorkplacePosition, PossessedCitizen->GetWorkplace()->GetActorLocation());
		}
		if (!PossessedCitizen->IsHomeless())
		{
			BlackboardComponent->SetValueAsVector(BBKey_HousePosition, PossessedCitizen->GetHome()->GetActorLocation());
		}

		PossessedCitizen->OnHomeAssigned.AddUniqueDynamic(this, &ThisClass::OnPawnAssignedHome);
		PossessedCitizen->OnWorkplaceAssigned.AddUniqueDynamic(this, &ThisClass::OnPawnAssignedWorkplace);
	}
}

void ACitizenAIController::OnPawnAssignedHome(AStructure* AssignedHome)
{
	ACitizen* PossessedCitizen = Cast<ACitizen>(GetPawn());
	BlackboardComponent->SetValueAsVector(BBKey_HousePosition, PossessedCitizen->GetHome()->GetActorLocation());
}

void ACitizenAIController::OnPawnAssignedWorkplace(AStructure* AssignedWorkplace)
{
	ACitizen* PossessedCitizen = Cast<ACitizen>(GetPawn());
	BlackboardComponent->SetValueAsVector(BBKey_WorkplacePosition, PossessedCitizen->GetWorkplace()->GetActorLocation());
}


