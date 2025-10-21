// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ResourceDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ResourcesSubsystem.generated.h"

enum class ECitizenType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceAddedSignature, UResourceDataAsset*, ResourceAdded, float, AmountAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceConsumedSignature, UResourceDataAsset*, ResourceConsumed, float, AmountConsumed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceStorageIncreasedSignature, UResourceDataAsset*, StoredResource, int32, AdditionalStorageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResourceStorageDecreasedSignature, UResourceDataAsset*, StoredResource, int32, DecreasedStorageAmount);  
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPopulationIncreasedSignature, ECitizenType, CitizenType, int32, NumOfCitizensAdded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPopulationDecreasedSignature, ECitizenType, CitizenType, int32, NumOfCitizensRemoved);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopulationCapacityIncreasedSignature, int32, IncreaseAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPopulationCapacityDecreasedSignature, int32, DecreaseAmount);

/**
 * 
 */
UCLASS(DisplayName="Resources Subsystem")
class STRATEGYGAME_API UResourcesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UResourcesSubsystem();

protected:
	
	UPROPERTY(VisibleAnywhere, Category="Resources Subsystem")
	TMap<UResourceDataAsset*, float> ResourceInventory;
	
	UPROPERTY(VisibleAnywhere, Category="Resources Subsystem")
	TMap<UResourceDataAsset*, int32> MaximumResources;
	
	UPROPERTY(VisibleAnywhere, Category="Resources Subsystem")
	TMap<ECitizenType, int32> Population;

	UPROPERTY(VisibleAnywhere, Category="Resources Subsystem")
	int32 PopulationCapacity = 0;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Ensures that resources are between 0 and their maximum capacity.
	void ClampResources();

public:
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FResourceAddedSignature OnResourceAdded;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FResourceConsumedSignature OnResourceConsumed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FResourceStorageIncreasedSignature OnResourceStorageIncreased;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FResourceStorageDecreasedSignature OnResourceStorageDecreased;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FPopulationIncreasedSignature OnPopulationIncreased;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FPopulationDecreasedSignature OnPopulationDecreased;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FPopulationCapacityIncreasedSignature OnPopulationCapacityIncreased;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Resources Subsystem")
	FPopulationCapacityDecreasedSignature OnPopulationCapacityDecreased;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Resources")
	float GetResourceAmount(UResourceDataAsset* InResource) const;

	// Gets the resource amount floored to an int32. Useful for UI and displaying resource counts.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Resources")
	int32 GetResourceAmountInt(UResourceDataAsset* InResource) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Resources")
	int32 GetResourceCapacity(UResourceDataAsset* InResource) const;

	// Returns a Text formatted for display, [ResourceName]: [ResourceAmount] / [ResourceCapacity].
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resource Subsystem|Resources")
	FText GetResourceText(UResourceDataAsset* InResource) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetPopulation(const ECitizenType WorkerType) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetTotalPopulation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetPopulationCapacity() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetEmployedPopulation(const ECitizenType CitizenType) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetUnemployedPopulation(const ECitizenType WorkerType) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetTotalEmployedPopulation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetTotalUnemployedPopulation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources Subsystem|Population")
	int32 GetHomelessPopulation() const;

	// Attempts to add resources to the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Resources")
	float AddResources(UResourceDataAsset* ResourceToAdd, const float AmountToAdd);

	// Attempts to remove resources from the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Resources")
	float ConsumeResources(UResourceDataAsset* ResourceToConsume, const float AmountToConsume);

	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Resources")
	int32 IncreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 IncreaseAmount);
	
	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Resources")
	int32 DecreaseResourceStorage(UResourceDataAsset* StoredResource, const int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Population")
	int32 IncreasePopulation(const ECitizenType CitizenType, const int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Population")
	int32 DecreasePopulation(const ECitizenType CitizenType, const int32 DecreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Population")
	int32 IncreasePopulationCapacity(const int32 IncreaseAmount);

	UFUNCTION(BlueprintCallable, Category="Resources Subsystem|Population")
	int32 DecreasePopulationCapacity(const int32 DecreaseAmount);
	
};
