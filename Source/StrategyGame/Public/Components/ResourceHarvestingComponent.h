// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureComponent.h"
#include "ResourceHarvestingComponent.generated.h"

class UWorkersComponent;
class UResourceDataAsset;
class USphereComponent;

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRATEGYGAME_API UResourceHarvestingComponent : public UStructureComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResourceHarvestingComponent();

protected:

	UPROPERTY()
	UWorkersComponent* OwningStructureWorkersComponent;

	UPROPERTY(EditAnywhere, DisplayName="ResourceHarvesterSphere")
	USphereComponent* SphereComponent;

	// How much of each listed resource to harvest each hour when at 100% worker capacity.
	UPROPERTY(EditAnywhere)
	TMap<UResourceDataAsset*, float> ResourcesToHarvestPerHour;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

	virtual void OnTimePassed(const float HoursPassed) override;

	void HarvestNearbyResources(const float HoursPassed);

public:

	UFUNCTION(BlueprintPure, Category="Resource Storage")
	TArray<UResourceDataAsset*> GetHarvestingResources() const;

	UFUNCTION(BlueprintPure, Category="Resource Storage")
	int32 GetResourceHarvestAmount(const UResourceDataAsset* InResourceType) const { return ResourcesToHarvestPerHour.FindRef(InResourceType); }
};
