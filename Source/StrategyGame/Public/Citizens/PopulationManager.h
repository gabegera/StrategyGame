// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopulationManager.generated.h"

class AStructure;
class ACitizen;

/**
 * Keeps track of all citizens in the city and assigns them homes and workplaces whenever needed.
 */
UCLASS(NotBlueprintable)
class STRATEGYGAME_API APopulationManager : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category="Population Manager")
	TSet<ACitizen*> Citizens;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStructureBuilt(AStructure* NewStructure);

	UFUNCTION()
	virtual void OnStructureDestroyed();

public:

	UFUNCTION(BlueprintPure, Category="Population Manager")
	TSet<ACitizen*>& GetCitizens();

	UFUNCTION(BlueprintPure, Category="Population Manager")
	TSet<ACitizen*> GetUnemployedCitizens();

	UFUNCTION(BlueprintPure, Category="Population Manager")
	TSet<ACitizen*> GetEmployedCitizens();

	UFUNCTION(BlueprintPure, Category="Population Manager")
	TSet<ACitizen*> GetHomelessCitizens();

	UFUNCTION(BlueprintPure, Category="Population Manager")
	TSet<ACitizen*> GetHousedCitizens();
};
