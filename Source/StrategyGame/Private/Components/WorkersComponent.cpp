// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WorkersComponent.h"

#include "StrategyEnums.h"
#include "Citizens/Citizen.h"
#include "Game/StrategyGameInstance.h"


// Sets default values for this component's properties
UWorkersComponent::UWorkersComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FetchAllCitizenTypes();
}


// Called when the game starts
void UWorkersComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UWorkersComponent::FetchAllCitizenTypes()
{
	const int32 NumOfCitizenTypes = StaticEnum<ECitizenType>()->NumEnums();
	for (int32 i = 0; i < NumOfCitizenTypes - 1; i++)
	{
		AllowedWorkerTypes.FindOrAdd(static_cast<ECitizenType>(i), true);
	}
	AllowedWorkerTypes.KeySort([](const ECitizenType& A, const ECitizenType& B)
	{
		return A < B;
	});
}

void UWorkersComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FetchAllCitizenTypes();
}

void UWorkersComponent::RequestWorker(const ECitizenType RequestedWorkerType)
{
	if (OwningStructure && !IsFullCapacity() && IsWorkerAllowed(RequestedWorkerType))
	{
		GetWorld()->GetGameInstance<UStrategyGameInstance>()->OnWorkerRequested.Broadcast(OwningStructure, this, RequestedWorkerType);
	}
}

void UWorkersComponent::RequestNumOfWorkers(const int32 RequestedNumOfWorkers, const ECitizenType RequestedWorkerType)
{
	for (int32 i = 0; i < RequestedNumOfWorkers; i++)
	{
		RequestWorker(RequestedWorkerType);
	}
}

void UWorkersComponent::OnStructureBuilt(AStructure* BuiltStructure)
{
	Super::OnStructureBuilt(BuiltStructure);

	if (BuiltStructure == OwningStructure)
	{
		RequestNumOfWorkers(MaxNumOfWorkers, ECitizenType::Worker);
	}
}

void UWorkersComponent::AssignWorker(ACitizen* InWorker)
{
	if (!IsFullCapacity())
	{
		AssignedWorkers.Add(InWorker);
	}
}

bool UWorkersComponent::IsFullCapacity() const
{
	return AssignedWorkers.Num() >= MaxNumOfWorkers;
}

bool UWorkersComponent::IsWorkerAllowed(const ECitizenType InCitizenType) const
{
	return AllowedWorkerTypes.FindRef(InCitizenType) == true;
}

int32 UWorkersComponent::GetMaxNumOfWorkers() const
{
	return MaxNumOfWorkers;
}

int32 UWorkersComponent::GetNumOfWorkingWorkers() const
{
	int32 OutNum = 0;

	for (const ACitizen* Worker : AssignedWorkers)
	{
		if (Worker->GetCitizenState() == ECitizenState::Working)
		{
			OutNum++;
		}
	}

	return OutNum;
}

TSet<ACitizen*> UWorkersComponent::GetAssignedWorkers() const
{
	return AssignedWorkers;
}

int32 UWorkersComponent::GetNumOfAssignedWorkers(const ECitizenType InCitizenType) const
{
	int32 OutNum = 0;

	TArray<ACitizen*> WorkersArray = AssignedWorkers.Array();
	for (int32 i = 0; i < WorkersArray.Num(); i++)
	{
		if (WorkersArray[i]->GetCitizenType() == InCitizenType)
		{
			OutNum++;
		}
	}

	return OutNum;
}

float UWorkersComponent::GetWorkersPercentage() const
{
	return static_cast<float>(GetNumOfWorkingWorkers()) / MaxNumOfWorkers;
}
