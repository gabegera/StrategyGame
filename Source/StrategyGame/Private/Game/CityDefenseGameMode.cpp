// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CityDefenseGameMode.h"

#include "StrategyEnums.h"
#include "StrategyStatics.h"
#include "Citizens/CitizenSpawn.h"
#include "Citizens/PopulationManager.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/StrategyGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACityDefenseGameMode::ACityDefenseGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ACityDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!StartingCitizens.IsEmpty() && !GetGameInstance<UStrategyGameInstance>()->WasSaveLoaded())
	{
		const int32 NumOfWorkers = StartingCitizens.FindRef(ECitizenType::Worker);
		const int32 NumOfScientists = StartingCitizens.FindRef(ECitizenType::Scientist);
		SpawnCitizens(NumOfWorkers, NumOfScientists);
	}
}

void ACityDefenseGameMode::PostInitProperties()
{
	Super::PostInitProperties();

	bFindAllResources = false;
}

void ACityDefenseGameMode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (bFindAllResources)
	{
		PopulateAllResources();
	}
}

void ACityDefenseGameMode::PopulateAllResources()
{
	for (UResourceDataAsset* Resource : UStrategyStatics::GetAllResources())
	{
		StartingResources.FindOrAdd(Resource);
		StartingMaximumResources.FindOrAdd(Resource);

		StartingResources.KeySort([](const UResourceDataAsset& A, const UResourceDataAsset& B)
		{
			return A.GetResourceName().ToString() < B.GetResourceName().ToString();
		});

		StartingMaximumResources.KeySort([](const UResourceDataAsset& A, const UResourceDataAsset& B)
		{
			return A.GetResourceName().ToString() < B.GetResourceName().ToString();
		});
	}

	bFindAllResources = false;
}

void ACityDefenseGameMode::SpawnCitizens(const int32 NumOfWorkers, const int32 NumOfScientists) const
{
	ACitizenSpawn* CitizenSpawn =  Cast<ACitizenSpawn>(UGameplayStatics::GetActorOfClass(GetWorld(), ACitizenSpawn::StaticClass()));

	if (!CitizenSpawn)
	{
		CitizenSpawn = GetWorld()->SpawnActor<ACitizenSpawn>(ACitizenSpawn::StaticClass());
	}
	
	if (CitizenSpawn)
	{
		CitizenSpawn->SpawnCitizensInGrid(NumOfWorkers, NumOfScientists);
	}
}

TMap<UResourceDataAsset*, float>& ACityDefenseGameMode::GetStartingResources()
{
	return StartingResources;
}

TMap<UResourceDataAsset*, int32>& ACityDefenseGameMode::GetStartingMaxResources()
{
	return StartingMaximumResources;
}
