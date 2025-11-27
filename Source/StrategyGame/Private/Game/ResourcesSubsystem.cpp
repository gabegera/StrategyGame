// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ResourcesSubsystem.h"

#include "StrategyEnums.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "Components/ResourceStorageComponent.h"
#include "Game/StrategyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UResourcesSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UStrategyGameInstance* GameInstance = GetWorld()->GetGameInstance<UStrategyGameInstance>();
	GameInstance->OnStructureBuilt.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
	GameInstance->OnStructureDestroyed.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ThisClass::OnBeginPlay);
}

void UResourcesSubsystem::ClampAllResources()
{
	if (ResourceInventory.IsEmpty()) return;

	TArray<UResourceDataAsset*> Keys;
	ResourceInventory.GetKeys(Keys);
	for (UResourceDataAsset* Key : Keys)
	{
		ClampResource(Key);
	}
}

void UResourcesSubsystem::ClampResource(UResourceDataAsset* InResource)
{
	ResourceInventory.Add(InResource, FMath::Clamp(GetResourceAmount(InResource), 0.0f, static_cast<float>(GetResourceCapacity(InResource))));
}

void UResourcesSubsystem::OnBeginPlay()
{
	if (ACityDefenseGameMode* GameMode = Cast<ACityDefenseGameMode>(GetWorld()->GetAuthGameMode()))
	{
		ResourceInventory = GameMode->GetStartingResources();
	}

	UpdateStorageCapacity();
}

void UResourcesSubsystem::OnStructureBuilt(AStructure* NewStructure)
{
	if (NewStructure && NewStructure->DoesIncreaseStorage())
	{
		UpdateStorageCapacity();
	}
}

void UResourcesSubsystem::OnStructureDestroyed(AStructure* NewStructure)
{
	if (NewStructure && NewStructure->DoesIncreaseStorage())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::UpdateStorageCapacity);
	}
}

TMap<UResourceDataAsset*, float>& UResourcesSubsystem::SetResourceInventory(const TMap<UResourceDataAsset*, float> InResources)
{
	return ResourceInventory = InResources;
}

TMap<UResourceDataAsset*, float>& UResourcesSubsystem::GetResourceInventory()
{
	return ResourceInventory;
}

float UResourcesSubsystem::GetResourceAmount(UResourceDataAsset* InResource)
{
	return ResourceInventory.FindOrAdd(InResource);
}

int32 UResourcesSubsystem::GetResourceAmountInt(UResourceDataAsset* InResource)
{
	return FMath::FloorToInt32(GetResourceAmount(InResource));
}

TMap<UResourceDataAsset*, int32>& UResourcesSubsystem::SetResourceCapacity(const TMap<UResourceDataAsset*, int32> InCapacity)
{
	return MaximumResources = InCapacity;
}

int32 UResourcesSubsystem::GetResourceCapacity(UResourceDataAsset* InResource)
{
	return MaximumResources.FindOrAdd(InResource);
}

FText UResourcesSubsystem::GetResourceText(UResourceDataAsset* InResource)
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

int32 UResourcesSubsystem::GetPopulation(const ECitizenType WorkerType)
{
	return Population.FindOrAdd(WorkerType);
}

int32 UResourcesSubsystem::GetTotalPopulation()
{
	return GetPopulation(ECitizenType::Worker) + GetPopulation(ECitizenType::Scientist);
}

int32 UResourcesSubsystem::GetPopulationCapacity()
{
	return PopulationCapacity;
}

int32 UResourcesSubsystem::GetEmployedPopulation(ECitizenType CitizenType)
{
	int32 EmployedWorkers = 0;

	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetWorkerCount(CitizenType);
	// }

	return EmployedWorkers;
}

int32 UResourcesSubsystem::GetUnemployedPopulation(const ECitizenType WorkerType)
{
	return GetPopulation(WorkerType) - GetEmployedPopulation(WorkerType);
}

int32 UResourcesSubsystem::GetTotalEmployedPopulation() {
	int32 EmployedWorkers = 0;

	// for (auto Structure : BuiltStructures)
	// {
	// 	EmployedWorkers += Structure->GetTotalWorkers();
	// }

	return EmployedWorkers;
}

int32 UResourcesSubsystem::GetTotalUnemployedPopulation()
{
	return GetTotalPopulation() - GetTotalEmployedPopulation();
}

int32 UResourcesSubsystem::GetHomelessPopulation()
{
	if (GetTotalPopulation() < PopulationCapacity) return 0;

	return GetTotalPopulation() - PopulationCapacity;
}

float UResourcesSubsystem::AddResources(UResourceDataAsset* ResourceToAdd, const float AmountToAdd)
{
	if (ResourceToAdd)
	{
		ResourceInventory.Add(ResourceToAdd, GetResourceAmount(ResourceToAdd) + AmountToAdd);

		ClampAllResources();
	}

	OnResourceAdded.Broadcast(ResourceToAdd, AmountToAdd);
	return GetResourceAmount(ResourceToAdd);
}

float UResourcesSubsystem::AddResources(UResourceDataAsset* ResourceToAdd, const int32 AmountToAdd)
{
	return AddResources(ResourceToAdd, static_cast<float>(AmountToAdd));
}

float UResourcesSubsystem::ConsumeResources(UResourceDataAsset* ResourceToConsume, const float AmountToConsume)
{
	ResourceInventory.Add(ResourceToConsume, GetResourceAmount(ResourceToConsume) - AmountToConsume);

	ClampAllResources();

	OnResourceConsumed.Broadcast(ResourceToConsume, AmountToConsume);
	return GetResourceAmount(ResourceToConsume);
}

int32 UResourcesSubsystem::IncreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 IncreaseAmount)
{
	MaximumResources.Add(StoredResource, GetResourceCapacity(StoredResource) + IncreaseAmount);
	ClampAllResources();
	OnResourceStorageIncreased.Broadcast(StoredResource, IncreaseAmount);
	return GetResourceCapacity(StoredResource);
}

int32 UResourcesSubsystem::DecreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 DecreaseAmount)
{
	MaximumResources.Add(StoredResource, FMath::Clamp(GetResourceCapacity(StoredResource) - DecreaseAmount, 0, GetResourceCapacity(StoredResource)));
	ClampAllResources();
	OnResourceStorageDecreased.Broadcast(StoredResource, DecreaseAmount);
	return GetResourceCapacity(StoredResource);
}

void UResourcesSubsystem::UpdateStorageCapacity()
{
	TMap<UResourceDataAsset*, int32> UpdatedCapacity;
	if (ACityDefenseGameMode* GameMode = Cast<ACityDefenseGameMode>(GetWorld()->GetAuthGameMode()))
	{
		UpdatedCapacity = GameMode->GetStartingMaxResources();
	}

	TArray<AActor*> AllStructures;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), AllStructures);

	// TODO: I'm gonna need to find a more efficient way of doing this but for now this works.

	for (AActor* Structure : AllStructures)
	{
		IStructureInterface* StructureInterface = Cast<IStructureInterface>(Structure);
		if (StructureInterface && StructureInterface->DoesIncreaseStorage())
		{
			UResourceStorageComponent* ResourceStorageComponent =  Structure->GetComponentByClass<UResourceStorageComponent>();
			for (UResourceDataAsset* Resource : ResourceStorageComponent->GetStorageResources())
			{
				UpdatedCapacity.Add(Resource, UpdatedCapacity.FindOrAdd(Resource) + ResourceStorageComponent->GetResourceStorageAmount(Resource));
			}
		}
	}

	MaximumResources = UpdatedCapacity;

	ClampAllResources();
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
