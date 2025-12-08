// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/CitizenAIController.h"

#include "NavigationSystem.h"
#include "StrategyEnums.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "Game/TimeSubsystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Navigation/PathFollowingComponent.h"

enum class ECitizenState : uint8;

// Sets default values
ACitizenAIController::ACitizenAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeFinder(TEXT("/Game/Blueprints/AI/Citizens/BT_Citizen.BT_Citizen"));
	// if (BehaviorTreeFinder.Succeeded())
	// {
	// 	BehaviorTree = BehaviorTreeFinder.Object;
	// }
}

// Called when the game starts or when spawned
void ACitizenAIController::BeginPlay()
{
	Super::BeginPlay();

	UTimeSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();
	TimeSubsystem->OnTimePassed.AddUniqueDynamic(this, &ThisClass::OnTimePassed);
	TimeSubsystem->OnWorkTimeStarted.AddUniqueDynamic(this, &ThisClass::OnWorkTimeStarted);
	TimeSubsystem->OnWorkTimeEnded.AddUniqueDynamic(this, &ThisClass::OnWorkTimeEnded);
}

void ACitizenAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CitizenPawn = Cast<ACitizen>(InPawn);

	CitizenPawn->OnHomeAssigned.AddUniqueDynamic(this, &ThisClass::OnPawnAssignedHome);
    CitizenPawn->OnHomeCleared.AddUniqueDynamic(this, &ThisClass::OnPawnClearedHome);
    CitizenPawn->OnWorkplaceAssigned.AddUniqueDynamic(this, &ThisClass::OnPawnAssignedWorkplace);
    CitizenPawn->OnWorkplaceCleared.AddUniqueDynamic(this, &ThisClass::OnPawnClearedWorkplace);

	// ACitizen* PossessedCitizen = Cast<ACitizen>(InPawn);
	// if (PossessedCitizen && BehaviorTree)
	// {
	// 	UseBlackboard(BehaviorTree->GetBlackboardAsset(), BlackboardComponent);
	// 	RunBehaviorTree(BehaviorTree);
	// }
}

void ACitizenAIController::OnTimePassed(float HoursPassed)
{
	if (CitizenPawn)
	{
		constexpr float DistanceToEnterStructure = 200.0f;

		switch (CitizenPawn->GetCitizenState())
		{
		case ECitizenState::Roaming:
			if (IsTimeToWork() && CitizenPawn->IsEmployed())
			{
				GoToWork();
			}
			else if (!CitizenPawn->IsHomeless())
			{
				GoToHome();
			}
			break;

		case ECitizenState::GoingHome:
			if (GetDistanceToStructureEntrance(CitizenPawn->GetHome()) < DistanceToEnterStructure)
			{
				CitizenPawn->EnterStructure(CitizenPawn->GetHome());
				CitizenPawn->SetCitizenState(ECitizenState::AtHome);
			}
			break;

		case ECitizenState::AtHome:
			if (IsTimeToWork() && CitizenPawn->IsEmployed())
			{
				GoToWork();
			}
			break;

		case ECitizenState::GoingToWork:
			if (GetDistanceToStructureEntrance(CitizenPawn->GetWorkplace()) < DistanceToEnterStructure)
			{
				CitizenPawn->EnterStructure(CitizenPawn->GetWorkplace());
				CitizenPawn->SetCitizenState(ECitizenState::Working);
			}
			break;

		case ECitizenState::Working:
			if (!IsTimeToWork())
			{
				if (!CitizenPawn->IsHomeless())
				{
					GoToHome();
				}
				else
				{
					MoveToRandomPointInRadius(CitizenPawn->GetActorLocation(), RoamingRadius);
				}
			}
			break;

		default:
			break;
		}
	}
}

void ACitizenAIController::OnWorkTimeStarted()
{

}

void ACitizenAIController::OnWorkTimeEnded()
{

}

void ACitizenAIController::OnPawnAssignedHome(AStructure* AssignedHome)
{
	if (!IsTimeToWork() || !CitizenPawn->IsEmployed())
	{
		GoToHome();
	}
}

void ACitizenAIController::OnPawnClearedHome()
{
	CitizenPawn->ExitStructure();
	StopMovement();
	Roam();
}

void ACitizenAIController::OnPawnAssignedWorkplace(AStructure* AssignedWorkplace)
{
	// BlackboardComponent->SetValueAsVector(BBKey_WorkplaceLocation, AssignedWorkplace->GetEntranceLocation());

	if (IsTimeToWork())
	{
		GoToWork();
	}
}

void ACitizenAIController::OnPawnClearedWorkplace()
{
	if (!CitizenPawn) return;

	CitizenPawn->ExitStructure();
	StopMovement();

	if (CitizenPawn->IsHomeless())
	{
		Roam();
	}
	else
	{
		GoToHome();
	}
}

void ACitizenAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (CitizenPawn && CitizenPawn->GetCitizenState() == ECitizenState::Roaming)
	{
		const float TimerDuration = FMath::RandRange(MinTimeBetweenRoam, MaxTimeBetweenRoam);
		GetWorld()->GetTimerManager().SetTimer(RoamingDelayTimer, this, &ThisClass::Roam, TimerDuration);
	}
}

void ACitizenAIController::GoToHome()
{
	if (CitizenPawn && CitizenPawn->GetHome())
	{
		CitizenPawn->ExitStructure();
		const FVector TargetLocation = CitizenPawn->GetHome()->GetEntranceLocation();
		CitizenPawn->SetCitizenState(ECitizenState::GoingHome);
		MoveToLocation(TargetLocation, 50.0f);
	}
}

void ACitizenAIController::GoToWork()
{
	if (CitizenPawn && CitizenPawn->GetWorkplace())
	{
		CitizenPawn->ExitStructure();
		const FVector TargetLocation = CitizenPawn->GetWorkplace()->GetEntranceLocation();
		CitizenPawn->SetCitizenState(ECitizenState::GoingToWork);
		MoveToLocation(TargetLocation, 50.0f);
	}
}

void ACitizenAIController::Roam()
{
	CitizenPawn->SetCitizenState(ECitizenState::Roaming);
	MoveToRandomPointInRadius(CitizenPawn->GetActorLocation(), RoamingRadius);
}

void ACitizenAIController::MoveToRandomPointInRadius(const FVector Origin, const float Radius)
{
	if (!CitizenPawn) return;

	if (const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		FNavLocation OutRandomLocation;
		NavSystem->GetRandomReachablePointInRadius(Origin, Radius, OutRandomLocation);
		MoveToLocation(OutRandomLocation.Location);
	}
}

float ACitizenAIController::GetDistanceToStructureEntrance(const AStructure* InStructure) const
{
	if (CitizenPawn && InStructure)
	{
		return UKismetMathLibrary::Vector_Distance(CitizenPawn->GetActorLocation(), InStructure->GetEntranceLocation());
	}

	return 100000.0f;
}

ACitizen* ACitizenAIController::GetCitizenPawn() const
{
	return CitizenPawn;
}

bool ACitizenAIController::IsTimeToWork() const
{
	return GetGameInstance()->GetSubsystem<UTimeSubsystem>()->IsTimeToWork();
}


