// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyStatics.h"

#include "Citizens/Citizen.h"
#include "Citizens/CitizenSpawn.h"
#include "Game/StrategyGameInstance.h"
#include "Game/ResourcesSubsystem.h"
#include "Kismet/GameplayStatics.h"

UStrategyStatics::UStrategyStatics()
{
	ConstructorHelpers::FClassFinder<ACitizen> CitizenClassFinder(TEXT("/Game/Blueprints/AI/Citizens/BP_Citizen.BP_Citizen_C"));
	if (CitizenClassFinder.Succeeded())
	{
		CitizenClassToSpawn = CitizenClassFinder.Class;
	}
	else
	{
		CitizenClassToSpawn = ACitizen::StaticClass();
	}
}

int32 UStrategyStatics::GetGridSize(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (const UStrategyGameInstance* GameInstance = World->GetGameInstance<UStrategyGameInstance>())
	{
		return GameInstance->GetGridSize();
	}
	
	return 1;
}

void UStrategyStatics::SpawnCitizen(const UObject* WorldContextObject, ECitizenType CitizenType)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	const ACitizenSpawn* CivilianSpawn = Cast<ACitizenSpawn>(UGameplayStatics::GetActorOfClass(World, ACitizenSpawn::StaticClass()));
	FTransform SpawnTransform;
	if (CivilianSpawn)
	{
		SpawnTransform = CivilianSpawn->GetActorTransform();
	}
	
	ACitizen* NewCivilian = World->SpawnActorDeferred<ACitizen>(CitizenClassToSpawn, SpawnTransform);
	NewCivilian->SetCitizenType(CitizenType);
	NewCivilian->FinishSpawning(SpawnTransform);
}

void UStrategyStatics::SpawnCitizensMulti(const UObject* WorldContextObject, const int32 NumOfWorkers, const int32 NumOfScientists)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World) return;

	for (int32 i = 0; i < NumOfWorkers; i++)
	{
		SpawnCitizen(World, ECitizenType::Worker);
	}

	for (int32 i = 0; i < NumOfScientists; i++)
	{
		SpawnCitizen(World, ECitizenType::Scientist);
	}
}