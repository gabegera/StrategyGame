// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/CityDefenseGameMode.h"

#include "StrategyStatics.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "Citizens/PopulationManager.h"
#include "Game/UpgradesSubsystem.h"

ACityDefenseGameMode::ACityDefenseGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	StartingCitizens.Add(ECitizenType::Worker, 0);
	StartingCitizens.Add(ECitizenType::Scientist, 0);
}

void ACityDefenseGameMode::BeginPlay()
{
	Super::BeginPlay();

	for (const TSubclassOf Structure : StartingStructures)
	{
		if (Structure)
		{
			GetWorld()->GetGameInstance()->GetSubsystem<UUpgradesSubsystem>()->UnlockStructure(Structure);
		}
	}

	if (!StartingCitizens.IsEmpty())
	{
		const int32 NumOfWorkers = StartingCitizens.FindRef(ECitizenType::Worker);
		const int32 NumOfScientists = StartingCitizens.FindRef(ECitizenType::Scientist);
		UStrategyStatics::SpawnCitizensMulti(GetWorld(), NumOfWorkers, NumOfScientists);
	}

	GetWorld()->SpawnActor(APopulationManager::StaticClass());
}
