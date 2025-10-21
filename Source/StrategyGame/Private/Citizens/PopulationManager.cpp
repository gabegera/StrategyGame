// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/PopulationManager.h"

#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "Components/HousingComponent.h"
#include "Game/StrategyGameInstance.h"
#include "Kismet/GameplayStatics.h"

void APopulationManager::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> CitizensArray;
    	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACitizen::StaticClass(), CitizensArray);
    
    	for (AActor* Actor : CitizensArray)
    	{
    		Citizens.Add(Cast<ACitizen>(Actor));
    	}
    
    	GetWorld()->GetGameInstance<UStrategyGameInstance>()->OnStructureBuilt.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
    	GetWorld()->GetGameInstance<UStrategyGameInstance>()->OnStructureDestroyed.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);
}

void APopulationManager::OnStructureBuilt(AStructure* NewStructure)
{
	if (UHousingComponent* HousingComponent = NewStructure->GetComponentByClass<UHousingComponent>())
	{
		if (HousingComponent->IsFullCapacity()) return;
		
		TArray<ACitizen*> HomelessArray = GetHomelessCitizens().Array();
		int32 NumIterations = HousingComponent->GetNumAvailableResidents();
		if (HomelessArray.Num() < HousingComponent->GetNumAvailableResidents())
		{
			NumIterations = HomelessArray.Num();
		}
		
		for (int32 i = 0; i < NumIterations; i++)
		{
			HousingComponent->AssignResident(HomelessArray[i]);
			HomelessArray[i]->AssignHome(NewStructure);
		}
	}
}

void APopulationManager::OnStructureDestroyed()
{
	
}

TSet<ACitizen*>& APopulationManager::GetCitizens()
{
	return Citizens;
}

TSet<ACitizen*> APopulationManager::GetUnemployedCitizens()
{
	TSet<ACitizen*> OutSet;
	
	for (ACitizen* Citizen : Citizens)
	{
		if (!Citizen->IsEmployed())
		{
			OutSet.Add(Citizen);
		}
	}

	return OutSet;
}

TSet<ACitizen*> APopulationManager::GetEmployedCitizens()
{
	TSet<ACitizen*> OutSet;
	
	for (ACitizen* Citizen : Citizens)
	{
		if (Citizen->IsEmployed())
		{
			OutSet.Add(Citizen);
		}
	}

	return OutSet;
}

TSet<ACitizen*> APopulationManager::GetHomelessCitizens()
{
	TSet<ACitizen*> OutSet;
	
	for (ACitizen* Citizen : Citizens)
	{
		if (Citizen->IsHomeless())
		{
			OutSet.Add(Citizen);
		}
	}

	return OutSet;
}

TSet<ACitizen*> APopulationManager::GetHousedCitizens()
{
	TSet<ACitizen*> OutSet;
	
	for (ACitizen* Citizen : Citizens)
	{
		if (!Citizen->IsHomeless())
		{
			OutSet.Add(Citizen);
		}
	}

	return OutSet;
}
