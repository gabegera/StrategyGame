// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StrategyGameState.h"

#include "Building/Structure.h"
#include "Kismet/GameplayStatics.h"


AStrategyGameState::AStrategyGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	
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

	StructureBuiltDelegate.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
	StructureDestroyedDelegate.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);
}

void AStrategyGameState::BeginPlay()
{
	Super::BeginPlay();

	ClampResources();

	BuiltStructures = FindAllStructures();
}

void AStrategyGameState::OnStructureBuilt(AStructure* BuiltStructure)
{
	BuiltStructures.AddUnique(BuiltStructure);
}

void AStrategyGameState::OnStructureDestroyed(AStructure* BuiltStructure)
{
	BuiltStructures.Remove(BuiltStructure);
	BuiltStructures.Shrink();
}

void AStrategyGameState::ClampResources()
{
	for (auto Resource : ResourceInventory)
	{
		EResourceType ResourceType = Resource.Key;
		float Amount = Resource.Value;
		
		if (Amount > GetResourceCapacity(ResourceType))
		{
			ResourceInventory.Add(ResourceType, GetResourceCapacity(ResourceType));
		}
		else if (Amount < 0)
		{
			ResourceInventory.Add(ResourceType, 0);
		}
	}
}

TArray<AStructure*> AStrategyGameState::FindAllStructures()
{
	TArray<AActor*> FoundStructures;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), FoundStructures);

	TArray<AStructure*> OutArray;
	for (AActor* FoundStructure : FoundStructures)
	{
		OutArray.AddUnique(Cast<AStructure>(FoundStructure));
	}

	return OutArray;
}

void AStrategyGameState::UpdateTimeOfDay(float DeltaSeconds)
{
	switch (TimeScale)
	{
	case ETimeScale::OneTimesSpeed:
		TimeOfDay += DeltaSeconds / GetStrategyGameMode()->GetSecondsInGameHours();
		break;
	case ETimeScale::TwoTimesSpeed:
		TimeOfDay += (DeltaSeconds / GetStrategyGameMode()->GetSecondsInGameHours()) * 2.0f;
		break;
	case ETimeScale::ThreeTimesSpeed:
		TimeOfDay += (DeltaSeconds / GetStrategyGameMode()->GetSecondsInGameHours()) * 3.0f;
		break;
	}

	if (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
		DaysCitySurvived++;
	}
}

void AStrategyGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateTimeOfDay(DeltaSeconds);
}

AStrategyGameModeBase* AStrategyGameState::GetStrategyGameMode()
{
	if (StrategyGameMode == nullptr)
	{
		StrategyGameMode = Cast<AStrategyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	return StrategyGameMode;
}

ETimeScale AStrategyGameState::SetTimeScale(ETimeScale NewTimeScale)
{
	switch (NewTimeScale)
	{
	case ETimeScale::OneTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		break;
	case ETimeScale::TwoTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2.0f);
		break;
	case ETimeScale::ThreeTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 3.0f);
		break;
	}
	
	OnTimeScaleChanged.Broadcast(NewTimeScale);
	
	return TimeScale = NewTimeScale;
}

int32 AStrategyGameState::GetEmployedPopulation(ECitizenType WorkerType)
{
	int32 EmployedWorkers = 0;
	for (auto Structure : BuiltStructures)
	{
		EmployedWorkers += Structure->GetWorkerCount(WorkerType);
	}

	return EmployedWorkers;
}

int32 AStrategyGameState::GetTotalEmployedPopulation()
{
	int32 EmployedWorkers = 0;
	for (auto Structure : BuiltStructures)
	{
		EmployedWorkers += Structure->GetTotalWorkers();
	}

	return EmployedWorkers;
}

int32 AStrategyGameState::GetHomelessPopulation()
{
	if (GetTotalPopulation() < PopulationCapacity) return 0;

	return GetTotalPopulation() - PopulationCapacity;
}

float AStrategyGameState::AddResources(EResourceType ResourceType, float Amount)
{
	// Prints a debug message and returns if the resource storage is full.
	if (GetResourceAmount(ResourceType) == GetResourceCapacity(ResourceType))
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
		return GetResourceAmount(ResourceType);
	}
	
	ResourceInventory.Add(ResourceType, GetResourceAmount(ResourceType) + Amount);

	ClampResources();

	OnResourcesChanged.Broadcast();
	return GetResourceAmount(ResourceType);
}

float AStrategyGameState::ConsumeResources(EResourceType ResourceType, float Amount)
{
	// Prints a debug message and returns if attempting to remove more resources than are currently available.
	if (GetResourceAmount(ResourceType) - Amount < 0)
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
		return GetResourceAmount(ResourceType);
	}
	
	ResourceInventory.Add(ResourceType, GetResourceAmount(ResourceType) - Amount);

	ClampResources();

	OnResourcesChanged.Broadcast();
	return GetResourceAmount(ResourceType);
}

int32 AStrategyGameState::IncreaseResourceStorage(EResourceType ResourceType, int32 IncreaseAmount)
{
	MaximumResources.Add(ResourceType, GetResourceCapacity(ResourceType) + IncreaseAmount);
	ClampResources();
	OnResourcesChanged.Broadcast();
	return GetResourceCapacity(ResourceType);
}

int32 AStrategyGameState::DecreaseResourceStorage(EResourceType ResourceType, int32 DecreaseAmount)
{
	MaximumResources.Add(ResourceType, FMath::Clamp(GetResourceCapacity(ResourceType) - DecreaseAmount, 0, GetResourceCapacity(ResourceType)));
	ClampResources();
	OnResourcesChanged.Broadcast();
	return GetResourceCapacity(ResourceType);
}

int32 AStrategyGameState::IncreasePopulation(ECitizenType WorkerType, int32 IncreaseAmount)
{
	Population.Add(WorkerType, GetPopulation(WorkerType) + IncreaseAmount);

	OnPopulationChanged.Broadcast();
	return GetPopulation(WorkerType);
}

int32 AStrategyGameState::DecreasePopulation(ECitizenType WorkerType, int32 DecreaseAmount)
{
	Population.Add(WorkerType, GetPopulation(WorkerType) - DecreaseAmount);

	OnPopulationChanged.Broadcast();
	return GetPopulation(WorkerType);
}

int32 AStrategyGameState::IncreasePopulationCapacity(int32 IncreaseAmount)
{
	PopulationCapacity += IncreaseAmount;
	OnPopulationChanged.Broadcast();
	return PopulationCapacity;
}

int32 AStrategyGameState::DecreasePopulationCapacity(int32 DecreaseAmount)
{
	PopulationCapacity = FMath::Clamp(PopulationCapacity - DecreaseAmount, 0, PopulationCapacity);
	OnPopulationChanged.Broadcast();
	return PopulationCapacity;
	
}
