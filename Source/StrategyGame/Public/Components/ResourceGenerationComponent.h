// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceGenerationComponent.generated.h"

class UResourceDataAsset;
class UResourcesSubsystem;

UCLASS(ClassGroup=StructureComponents)
class STRATEGYGAME_API UResourceGenerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResourceGenerationComponent();

protected:

	UPROPERTY()
	UResourcesSubsystem* ResourcesSubsystem;
	
	UPROPERTY(EditAnywhere, Category="Resource Generation")
	TMap<UResourceDataAsset*, float> ResourcesToGeneratePerHour;

	// If set to true, requires a set amount of resources in order to generate new ones.
	UPROPERTY(EditAnywhere, Category="Resource Generation")
	bool bCostsResources = false;

	UPROPERTY(EditAnywhere, Category="Resource Generation", meta=(EditCondition=bCostsResources, EditConditionHides))
	TMap<UResourceDataAsset*, float> ResourcesToConsumePerHour;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTimePassed(float HoursPassed);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnTimePassed", Category="Resource Generation")
	void ReactToOnTimePassed(float HoursPassed);
	
	void GenerateResources(float HoursPassed);

	// Returns true if successful, false if not enough resources to consume.
	bool ConsumeResources(float HoursPassed);

public:
	
	UFUNCTION(BlueprintPure, Category="Resource Generation")
	TArray<UResourceDataAsset*> GetGeneratedResources() const;

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	float GetGeneratedResourceAmount(const UResourceDataAsset* InResource) const {return ResourcesToGeneratePerHour.FindRef(InResource); }

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	bool GetCostsResources() const { return bCostsResources; }

	UFUNCTION(BlueprintPure, Category="Resource Generation")
	bool HasEnoughResourcesToConsume(float HoursPassed) const;
};
