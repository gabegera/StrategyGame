// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CityResourcesSubsystem.generated.h"

UENUM(BlueprintType, DisplayName="Resource Type")
enum class EResourceType : uint8
{
	Metal						UMETA(DisplayName="Metal"),
	Concrete					UMETA(DisplayName="Concrete"),
	Oil							UMETA(DisplayName="Oil"),
	AlienMaterial				UMETA(DisplayName="Alien Material"),
	Food						UMETA(DisplayName="Food"),
	Power						UMETA(DisplayName="Power"),
	ResearchPoints				UMETA(DisplayName="Research Points"),
};

UENUM(BlueprintType, DisplayName="Citizen Type")
enum class ECitizenType : uint8
{
	Worker			UMETA(DisplayName="Worker"),
	Scientist		UMETA(DisplayName="Scientist"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResourcesChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPopulationChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAssignedWorkersChanged);

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UCityResourcesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCityResourcesSubsystem();

protected:
	
	static inline TMap<EResourceType, float> ResourceInventory;
	
	static inline TMap<EResourceType, int32> MaximumResources;
	
	static inline TMap<ECitizenType, int32> Population;
	
	static inline int32 PopulationCapacity = 0;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Ensures that resources are between 0 and their maximum capacity.
	static void ClampResources();

public:
	
	static inline FResourcesChanged OnResourcesChanged;
	
	static inline FResourcesChanged OnPopulationChanged;
	
	static inline FAssignedWorkersChanged OnAssignedWorkersChanged;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	static float GetCityResourceAmount(EResourceType ResourceType) { return ResourceInventory.FindRef(ResourceType); }

	// Gets the resource amount floored to an int32. Useful for UI and displaying resource counts.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	static int32 GetCityResourceAmountInt(EResourceType ResourceType) { return FMath::FloorToInt32(GetCityResourceAmount(ResourceType)); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	static int32 GetCityResourceCapacity(EResourceType ResourceType) { return MaximumResources.FindRef(ResourceType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityPopulation(ECitizenType WorkerType) { return Population.FindRef(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityTotalPopulation() { return GetCityPopulation(ECitizenType::Worker) + GetCityPopulation(ECitizenType::Scientist); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityPopulationCapacity() { return PopulationCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityEmployedPopulation(ECitizenType WorkerType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityUnemployedPopulation(ECitizenType WorkerType) { return GetCityPopulation(WorkerType) - GetCityEmployedPopulation(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityTotalEmployedPopulation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityTotalUnemployedPopulation() { return GetCityTotalPopulation() - GetCityTotalEmployedPopulation(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	static int32 GetCityHomelessPopulation();

	// Attempts to add resources to the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	static float AddCityResources(EResourceType ResourceType, float Amount);

	// Attempts to remove resources from the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	static float ConsumeCityResources(EResourceType ResourceType, float Amount);

	UFUNCTION(BlueprintCallable, Category="Resources")
	static int32 IncreaseCityResourceStorage(EResourceType ResourceType, int32 IncreaseAmount);
	
	UFUNCTION(BlueprintCallable, Category="Resources")
	static int32 DecreaseCityResourceStorage(EResourceType ResourceType, int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	static int32 IncreaseCityPopulation(ECitizenType WorkerType, int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	static int32 DecreaseCityPopulation(ECitizenType WorkerType, int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	static int32 IncreaseCityPopulationCapacity(int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	static int32 DecreaseCityPopulationCapacity(int32 DecreaseAmount);
	
};
