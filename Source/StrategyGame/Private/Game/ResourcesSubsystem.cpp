// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ResourcesSubsystem.h"

#include "Citizens/Citizen.h"

UResourcesSubsystem::UResourcesSubsystem()
{
	Population.Add(ECitizenType::Worker, 100);
	Population.Add(ECitizenType::Scientist, 20);
}

void UResourcesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ClampResources();
}

void UResourcesSubsystem::ClampResources()
{
	for (auto Resource : ResourceInventory)
	{
		UResourceDataAsset* ResourceData;
		ResourceData = Resource.Key;
		float Amount = Resource.Value;
		
		if (Amount > GetResourceCapacity(ResourceData))
		{
			ResourceInventory.Add(ResourceData, GetResourceCapacity(ResourceData));
		}
		else if (Amount < 0)
		{
			ResourceInventory.Add(ResourceData, 0);
		}
	}
}

float UResourcesSubsystem::GetResourceAmount(UResourceDataAsset* InResource) const
{
	if (ResourceInventory.Contains(InResource))
	{
		return ResourceInventory.FindRef(InResource);
	}
	
	return 0.0f;
}

int32 UResourcesSubsystem::GetResourceAmountInt(UResourceDataAsset* InResource) const
{
	return FMath::FloorToInt32(GetResourceAmount(InResource));
}

int32 UResourcesSubsystem::GetResourceCapacity(UResourceDataAsset* InResource) const
{
	if (MaximumResources.Contains(InResource))
	{
		return MaximumResources.FindRef(InResource);
	}
	
	return 0;
}

FText UResourcesSubsystem::GetResourceText(UResourceDataAsset* InResource) const
{
	if (!InResource)
	{
		return FText::FromString("ERROR: Invalid InResource.");
	}
	
	FString OutString = InResource->GetResourceName().ToString();
	OutString.Append(": ");
	OutString.AppendInt(GetResourceAmountInt(InResource));
	OutString.Append(" / ");
	OutString.AppendInt(GetResourceCapacity(InResource));

	return FText::FromString(OutString);
}

int32 UResourcesSubsystem::GetPopulation(ECitizenType WorkerType) const
{
	if (Population.Contains(WorkerType))
	{
		return Population.FindRef(WorkerType);
	}

	return 0;
}

int32 UResourcesSubsystem::GetTotalPopulation() const
{
	return GetPopulation(ECitizenType::Worker) + GetPopulation(ECitizenType::Scientist);
}

int32 UResourcesSubsystem::GetPopulationCapacity() const
{
	return PopulationCapacity;
}

int32 UResourcesSubsystem::GetEmployedPopulation(const ECitizenType CitizenType) const
{
	int32 EmployedWorkers = 0;
	
	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetWorkerCount(CitizenType);
	// }

	return EmployedWorkers;
}

int32 UResourcesSubsystem::GetUnemployedPopulation(const ECitizenType WorkerType) const
{
	return GetPopulation(WorkerType) - GetEmployedPopulation(WorkerType);
}

int32 UResourcesSubsystem::GetTotalEmployedPopulation() const
{
	int32 EmployedWorkers = 0;
	
	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetTotalWorkers();
	// }

	return EmployedWorkers;
}

int32 UResourcesSubsystem::GetTotalUnemployedPopulation() const
{
	return GetTotalPopulation() - GetTotalEmployedPopulation();
}

int32 UResourcesSubsystem::GetHomelessPopulation() const
{
	if (GetTotalPopulation() < PopulationCapacity) return 0;

	return GetTotalPopulation() - PopulationCapacity;
}

float UResourcesSubsystem::AddResources(UResourceDataAsset* ResourceToAdd, const float AmountToAdd)
{
	ResourceInventory.Add(ResourceToAdd, GetResourceAmount(ResourceToAdd) + AmountToAdd);

	ClampResources();

	OnResourceAdded.Broadcast(ResourceToAdd, AmountToAdd);
	return GetResourceAmount(ResourceToAdd);
}

float UResourcesSubsystem::ConsumeResources(UResourceDataAsset* ResourceToConsume, const float AmountToConsume)
{
	ResourceInventory.Add(ResourceToConsume, GetResourceAmount(ResourceToConsume) - AmountToConsume);

	ClampResources();

	OnResourceConsumed.Broadcast(ResourceToConsume, AmountToConsume);
	return GetResourceAmount(ResourceToConsume);
}

int32 UResourcesSubsystem::IncreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 IncreaseAmount)
{
	MaximumResources.Add(StoredResource, GetResourceCapacity(StoredResource) + IncreaseAmount);
	ClampResources();
	OnResourceStorageIncreased.Broadcast(StoredResource, IncreaseAmount);
	return GetResourceCapacity(StoredResource);
}

int32 UResourcesSubsystem::DecreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 DecreaseAmount)
{
	MaximumResources.Add(StoredResource, FMath::Clamp(GetResourceCapacity(StoredResource) - DecreaseAmount, 0, GetResourceCapacity(StoredResource)));
	ClampResources();
	OnResourceStorageDecreased.Broadcast(StoredResource, DecreaseAmount);
	return GetResourceCapacity(StoredResource);
}

int32 UResourcesSubsystem::IncreasePopulation(const ECitizenType CitizenType, const int32 IncreaseAmount)
{
	Population.Add(CitizenType, GetPopulation(CitizenType) + IncreaseAmount);

	OnPopulationIncreased.Broadcast(CitizenType, IncreaseAmount);
	return GetPopulation(CitizenType);
}

int32 UResourcesSubsystem::DecreasePopulation(const ECitizenType CitizenType, const int32 DecreaseAmount)
{
	Population.Add(CitizenType, GetPopulation(CitizenType) - DecreaseAmount);

	OnPopulationDecreased.Broadcast(CitizenType, DecreaseAmount);
	return GetPopulation(CitizenType);
}

int32 UResourcesSubsystem::IncreasePopulationCapacity(const int32 IncreaseAmount)
{
	PopulationCapacity += IncreaseAmount;
	OnPopulationCapacityIncreased.Broadcast(IncreaseAmount);
	return PopulationCapacity;
}

int32 UResourcesSubsystem::DecreasePopulationCapacity(const int32 DecreaseAmount)
{
	PopulationCapacity = FMath::Clamp(PopulationCapacity - DecreaseAmount, 0, PopulationCapacity);
	OnPopulationCapacityDecreased.Broadcast(DecreaseAmount);
	return PopulationCapacity;
	
}
