// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureComponent.h"
#include "ResourceGenerationComponent.generated.h"

class UWorkersComponent;
class UResourceDataAsset;
class UResourcesSubsystem;

UCLASS(BlueprintType, ClassGroup=StructureComponents)
class STRATEGYGAME_API UResourceGenerationComponent : public UStructureComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResourceGenerationComponent();

protected:

	UPROPERTY()
	UWorkersComponent* OwningStructureWorkersComponent;

	UPROPERTY()
	UResourcesSubsystem* ResourcesSubsystem;

	// How much of each listed resource to generate each hour when at 100% worker capacity.
	UPROPERTY(EditAnywhere, Category="Resource Generation")
	TMap<UResourceDataAsset*, float> ResourcesToGeneratePerHour;

	// If set to true, requires a set amount of resources in order to generate new ones.
	UPROPERTY(EditAnywhere, Category="Resource Generation")
	bool bCostsResources = false;

	// How much of each listed resource to consume each hour when at 100% worker capacity.
	UPROPERTY(EditAnywhere, Category="Resource Generation", meta=(EditCondition=bCostsResources, EditConditionHides))
	TMap<UResourceDataAsset*, float> ResourcesToConsumePerHour;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnTimePassed(float HoursPassed) override;
	
	void GenerateResources(float HoursPassed);

	// Returns true if successful, false if not enough resources to consume.
	bool ConsumeResources(float HoursPassed);

public:
	
	UFUNCTION(BlueprintPure, Category="Resource Generation")
	TArray<UResourceDataAsset*> GetGeneratedResources() const;

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	float GetGeneratedResourceAmount(const UResourceDataAsset* InResource) const;

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	bool GetCostsResources() const;

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	bool HasEnoughResourcesToConsume(float HoursPassed) const;
};
