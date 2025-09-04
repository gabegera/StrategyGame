// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StrategyGameState.generated.h"

class AStrategyGameModeBase;
class AStructure;
class ARoad;

UENUM(BlueprintType, DisplayName="Time Scale")
enum class ETimeScale : uint8
{
	OneTimesSpeed		UMETA(DisplayName="1x Speed"),
	TwoTimesSpeed		UMETA(DisplayName="2x Speed"),
	ThreeTimesSpeed		UMETA(DisplayName="3x Speed"),
	Paused				UMETA(DisplayName="Paused")
};

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeScaleChangedDelegate, ETimeScale, NewTimeScale);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FResourcesChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPopulationChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAssignedWorkersChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureBuiltDelegate, AStructure*, BuiltStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkyscraperModuleAddedDelegate, ASkyscraper*, Skyscraper, ASkyscraperModule*, AddedModule);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureDestroyedDelegate, AStructure*, DestroyedStructure);

UCLASS()
class STRATEGYGAME_API AStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

	AStrategyGameState();

protected:

	UPROPERTY()
	AStrategyGameModeBase* StrategyGameMode = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Time")
	ETimeScale TimeScale = ETimeScale::OneTimesSpeed;

	// The Time of Day in hours. Ranges between 0 and 24
	UPROPERTY(VisibleAnywhere, Category="Time")
	float TimeOfDay = 0.0f;

	// Number of days the city has survived.
	UPROPERTY(VisibleAnywhere, Category="Time")
	int32 DaysCitySurvived = 0;

	UPROPERTY() TArray<AStructure*> BuiltStructures;
	
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TMap<EResourceType, float> ResourceInventory;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TMap<EResourceType, int32> MaximumResources;

	UPROPERTY(EditDefaultsOnly, Category="Population")
	TMap<ECitizenType, int32> Population;

	UPROPERTY(EditDefaultsOnly, Category="Population")
	int32 PopulationCapacity = 0;

	virtual void BeginPlay() override;

	UFUNCTION() void OnStructureBuilt(AStructure* BuiltStructure);
	UFUNCTION() void OnStructureDestroyed(AStructure* BuiltStructure);

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTimeScaleChangedDelegate OnTimeScaleChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResourcesChangedDelegate OnResourcesChanged;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FResourcesChangedDelegate OnPopulationChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FAssignedWorkersChangedDelegate OnAssignedWorkersChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStructureBuiltDelegate StructureBuiltDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSkyscraperModuleAddedDelegate OnSkyscraperModuleAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStructureBuiltDelegate StructureDestroyedDelegate;

	// Ensures that resources are between 0 and their maximum capacity.
	UFUNCTION()
	void ClampResources();

	// Finds all the structures that have been placed and returns an array.
	UFUNCTION()
	TArray<AStructure*> FindAllStructures();

	UFUNCTION()
	void UpdateTimeOfDay(float DeltaSeconds);

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameModeBase* GetStrategyGameMode();

	UFUNCTION(BlueprintCallable, Category="Time")
	ETimeScale SetTimeScale(ETimeScale NewTimeScale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	ETimeScale GetTimeScale() { return TimeScale; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	float GetTimeOfDay() { return TimeOfDay; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	int32 GetDaysCityHasSurvived() { return DaysCitySurvived; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	float GetResourceAmount(EResourceType ResourceType) { return ResourceInventory.FindRef(ResourceType); }

	// Gets the resource amount floored to an int32. Useful for UI and displaying resource counts.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	int32 GetResourceAmountInt32(EResourceType ResourceType) { return FMath::FloorToInt32(GetResourceAmount(ResourceType)); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	int32 GetResourceCapacity(EResourceType ResourceType) { return MaximumResources.FindRef(ResourceType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetPopulation(ECitizenType WorkerType) { return Population.FindRef(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetTotalPopulation() { return GetPopulation(ECitizenType::Worker) + GetPopulation(ECitizenType::Scientist); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetPopulationCapacity() { return PopulationCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetEmployedPopulation(ECitizenType WorkerType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetUnemployedPopulation(ECitizenType WorkerType) { return GetPopulation(WorkerType) - GetEmployedPopulation(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetTotalEmployedPopulation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetTotalUnemployedPopulation() { return GetTotalPopulation() - GetTotalEmployedPopulation(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetHomelessPopulation();

	// Attempts to add resources to the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	float AddResources(EResourceType ResourceType, float Amount);

	// Attempts to remove resources from the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	float ConsumeResources(EResourceType ResourceType, float Amount);

	UFUNCTION(BlueprintCallable, Category="Resources")
	int32 IncreaseResourceStorage(EResourceType ResourceType, int32 IncreaseAmount);
	
	UFUNCTION(BlueprintCallable, Category="Resources")
	int32 DecreaseResourceStorage(EResourceType ResourceType, int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	int32 IncreasePopulation(ECitizenType WorkerType, int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	int32 DecreasePopulation(ECitizenType WorkerType, int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	int32 IncreasePopulationCapacity(int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Population")
	int32 DecreasePopulationCapacity(int32 DecreaseAmount);
};
