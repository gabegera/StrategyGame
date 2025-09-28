// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CityResourcesSubsystem.h"

UCityResourcesSubsystem::UCityResourcesSubsystem()
{
	ResourceInventory.Add(EResourceType::Metal, 40);
	ResourceInventory.Add(EResourceType::Concrete, 60);
	ResourceInventory.Add(EResourceType::Oil, 20);
	ResourceInventory.Add(EResourceType::AlienMaterial, 0);
	ResourceInventory.Add(EResourceType::Food, 20);
	ResourceInventory.Add(EResourceType::Power, 0);
	ResourceInventory.Add(EResourceType::ResearchPoints, 0);

	MaximumResources.Add(EResourceType::Metal, 100);
	MaximumResources.Add(EResourceType::Concrete, 100);
	MaximumResources.Add(EResourceType::Oil, 100);
	MaximumResources.Add(EResourceType::AlienMaterial, 20);
	MaximumResources.Add(EResourceType::Food, 100);
	MaximumResources.Add(EResourceType::Power, 200);
	MaximumResources.Add(EResourceType::ResearchPoints, 10);

	Population.Add(ECitizenType::Worker, 100);
	Population.Add(ECitizenType::Scientist, 20);
}

void UCityResourcesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ClampResources();
}

void UCityResourcesSubsystem::ClampResources()
{
	for (auto Resource : ResourceInventory)
	{
		EResourceType ResourceType = Resource.Key;
		float Amount = Resource.Value;
		
		if (Amount > GetCityResourceCapacity(ResourceType))
		{
			ResourceInventory.Add(ResourceType, GetCityResourceCapacity(ResourceType));
		}
		else if (Amount < 0)
		{
			ResourceInventory.Add(ResourceType, 0);
		}
	}
}

int32 UCityResourcesSubsystem::GetCityEmployedPopulation(ECitizenType WorkerType)
{
	int32 EmployedWorkers = 0;
	
	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetWorkerCount(WorkerType);
	// }

	return EmployedWorkers;
}

int32 UCityResourcesSubsystem::GetCityTotalEmployedPopulation()
{
	int32 EmployedWorkers = 0;
	
	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetTotalWorkers();
	// }

	return EmployedWorkers;
}

int32 UCityResourcesSubsystem::GetCityHomelessPopulation()
{
	if (GetCityTotalPopulation() < PopulationCapacity) return 0;

	return GetCityTotalPopulation() - PopulationCapacity;
}

float UCityResourcesSubsystem::AddCityResources(EResourceType ResourceType, float Amount)
{
	// Prints a debug message and returns if the resource storage is full.
	if (GetCityResourceAmount(ResourceType) == GetCityResourceCapacity(ResourceType))
	{
		switch (ResourceType)
		{
		case EResourceType::Metal:
			GEngine->AddOnScreenDebugMessage(900, 3.0f, FColor::Red, "METAL storage is full.");
			break;
		case EResourceType::Concrete:
			GEngine->AddOnScreenDebugMessage(900, 3.0f, FColor::Red, "CONCRETE storage is full.");
			break;
		case EResourceType::Oil:
			GEngine->AddOnScreenDebugMessage(900, 3.0f, FColor::Red, "OIL storage is full.");
			break;
		case EResourceType::AlienMaterial:
			GEngine->AddOnScreenDebugMessage(901, 3.0f, FColor::Red, "ALIEN MATERIAL storage is full.");
			break;
		case EResourceType::Food:
			GEngine->AddOnScreenDebugMessage(902, 3.0f, FColor::Red, "FOOD storage is full.");
			break;
		case EResourceType::Power:
			GEngine->AddOnScreenDebugMessage(903, 3.0f, FColor::Red, "POWER capacity is full.");
			break;
		case EResourceType::ResearchPoints:
			GEngine->AddOnScreenDebugMessage(904, 3.0f, FColor::Red, "RESEARCH POINTS capacity is full.");
			break;
		}
		return GetCityResourceAmount(ResourceType);
	}
	
	ResourceInventory.Add(ResourceType, GetCityResourceAmount(ResourceType) + Amount);

	ClampResources();

	OnResourcesChanged.Broadcast();
	return GetCityResourceAmount(ResourceType);
}

float UCityResourcesSubsystem::ConsumeCityResources(EResourceType ResourceType, float Amount)
{
	// Prints a debug message and returns if attempting to remove more resources than are currently available.
	if (GetCityResourceAmount(ResourceType) - Amount < 0)
	{
		switch (ResourceType)
		{
		case EResourceType::Metal:
			GEngine->AddOnScreenDebugMessage(906, 3.0f, FColor::Red, "Attempted to remove more METAL than was available.");
			break;
		case EResourceType::Concrete:
			GEngine->AddOnScreenDebugMessage(906, 3.0f, FColor::Red, "Attempted to remove more CONCRETE than was available.");
			break;
		case EResourceType::Oil:
			GEngine->AddOnScreenDebugMessage(906, 3.0f, FColor::Red, "Attempted to remove more OIL than was available.");
			break;
		case EResourceType::AlienMaterial:
			GEngine->AddOnScreenDebugMessage(907, 3.0f, FColor::Red, "Attempted to remove more ALIEN MATERIAL than was available.");
			break;
		case EResourceType::Food:
			GEngine->AddOnScreenDebugMessage(908, 3.0f, FColor::Red, "Attempted to remove more FOOD than was available.");
			break;
		case EResourceType::Power:
			GEngine->AddOnScreenDebugMessage(909, 3.0f, FColor::Red, "Attempted to remove more POWER than was available.");
			break;
		case EResourceType::ResearchPoints:
			GEngine->AddOnScreenDebugMessage(909, 3.0f, FColor::Red, "Attempted to remove more RESEARCH POINTS than was available.");
			break;
		}
		return GetCityResourceAmount(ResourceType);
	}
	
	ResourceInventory.Add(ResourceType, GetCityResourceAmount(ResourceType) - Amount);

	ClampResources();

	OnResourcesChanged.Broadcast();
	return GetCityResourceAmount(ResourceType);
}

int32 UCityResourcesSubsystem::IncreaseCityResourceStorage(EResourceType ResourceType, int32 IncreaseAmount)
{
	MaximumResources.Add(ResourceType, GetCityResourceCapacity(ResourceType) + IncreaseAmount);
	ClampResources();
	OnResourcesChanged.Broadcast();
	return GetCityResourceCapacity(ResourceType);
}

int32 UCityResourcesSubsystem::DecreaseCityResourceStorage(EResourceType ResourceType, int32 DecreaseAmount)
{
	MaximumResources.Add(ResourceType, FMath::Clamp(GetCityResourceCapacity(ResourceType) - DecreaseAmount, 0, GetCityResourceCapacity(ResourceType)));
	ClampResources();
	OnResourcesChanged.Broadcast();
	return GetCityResourceCapacity(ResourceType);
}

int32 UCityResourcesSubsystem::IncreaseCityPopulation(ECitizenType WorkerType, int32 IncreaseAmount)
{
	Population.Add(WorkerType, GetCityPopulation(WorkerType) + IncreaseAmount);

	OnPopulationChanged.Broadcast();
	return GetCityPopulation(WorkerType);
}

int32 UCityResourcesSubsystem::DecreaseCityPopulation(ECitizenType WorkerType, int32 DecreaseAmount)
{
	Population.Add(WorkerType, GetCityPopulation(WorkerType) - DecreaseAmount);

	OnPopulationChanged.Broadcast();
	return GetCityPopulation(WorkerType);
}

int32 UCityResourcesSubsystem::IncreaseCityPopulationCapacity(int32 IncreaseAmount)
{
	PopulationCapacity += IncreaseAmount;
	OnPopulationChanged.Broadcast();
	return PopulationCapacity;
}

int32 UCityResourcesSubsystem::DecreaseCityPopulationCapacity(int32 DecreaseAmount)
{
	PopulationCapacity = FMath::Clamp(PopulationCapacity - DecreaseAmount, 0, PopulationCapacity);
	OnPopulationChanged.Broadcast();
	return PopulationCapacity;
	
}
