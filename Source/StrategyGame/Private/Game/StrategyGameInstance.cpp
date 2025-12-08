// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StrategyGameInstance.h"

#include "StrategyEnums.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "Citizens/CitizenAIController.h"
#include "Components/HousingComponent.h"
#include "Components/WorkersComponent.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/StrategySaveGame.h"
#include "Game/TimeSubsystem.h"
#include "Game/UnlocksSubsystem.h"
#include "Kismet/GameplayStatics.h"

void UStrategyGameInstance::OnGameSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Game Saved");
}

void UStrategyGameInstance::OnSaveLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	const UStrategySaveGame* LoadedSave = Cast<UStrategySaveGame>(LoadedGameData);

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LoadedSave->Level);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorTransform(LoadedSave->PlayerSave.PlayerTransform);

	// For some reason the saved structures won't spawn unless scheduled for the next tick.
	FTimerDelegate LoadStructuresDelegate;
	LoadStructuresDelegate.BindUObject(this, &ThisClass::LoadSavedStructures, LoadedSave->SavedStructures);
	GetTimerManager().SetTimerForNextTick(LoadStructuresDelegate);

	FTimerDelegate LoadCitizensDelegate;
	LoadCitizensDelegate.BindUObject(this, &ThisClass::LoadSavedCitizens, LoadedSave->SavedCitizens);
	GetTimerManager().SetTimerForNextTick(LoadCitizensDelegate);

	GetSubsystem<UTimeSubsystem>()->SetTimeOfDay(LoadedSave->TimeOfDay);
	GetSubsystem<UTimeSubsystem>()->SetDaysCityHasSurvived(LoadedSave->DaysCityHasSurvived);
	GetSubsystem<UResourcesSubsystem>()->SetResourceInventory(LoadedSave->Resources);
	GetSubsystem<UUnlocksSubsystem>()->SetUnlockedUpgrades(LoadedSave->UnlockedUpgrades);

	UKismetSystemLibrary::PrintString(GetWorld(), "Save Loaded");

	bWasSaveLoaded = true;
}

void UStrategyGameInstance::LoadSavedStructures(const TArray<FStructureSave> SavedStructures)
{
	for (FStructureSave SavedStructure : SavedStructures)
	{
		if (!SavedStructure.StructureClass) continue;

		AStructure* SpawnedStructure = GetWorld()->SpawnActorDeferred<AStructure>(SavedStructure.StructureClass, SavedStructure.StructureTransform);
		SpawnedStructure->Rename(*SavedStructure.StructureName);
		SpawnedStructure->SetStructureState(SavedStructure.StructureState);
		SpawnedStructure->FinishSpawning(SavedStructure.StructureTransform);

		UKismetSystemLibrary::PrintString(GetWorld(), SavedStructure.ToString(), true, true, FLinearColor::Yellow, 5.0f);

		if (UWorkersComponent* WorkersComponent = SpawnedStructure->GetComponentByClass<UWorkersComponent>())
		{
			WorkersComponent->RequestNumOfWorkers(SavedStructure.NumOfHiredWorkers, ECitizenType::Worker);
			WorkersComponent->RequestNumOfWorkers(SavedStructure.NumOfHiredScientists, ECitizenType::Scientist);
		}
	}
}

void UStrategyGameInstance::LoadSavedCitizens(const TArray<FCitizenSave> SavedCitizens)
{
	for (FCitizenSave SavedCitizen : SavedCitizens)
	{
		ACitizen* SpawnedCitizen = GetWorld()->SpawnActorDeferred<ACitizen>(SavedCitizen.CitizenClass, FTransform());
		SpawnedCitizen->SetCitizenType(SavedCitizen.CitizenType);
		SpawnedCitizen->SetCitizenState(SavedCitizen.CitizenState);

		if (SavedCitizen.HomeName != "" || SavedCitizen.WorkplaceName != "")
		{
			TArray<AActor*> AllStructures;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), AllStructures);
			for (AActor* Structure : AllStructures)
			{
				if (Structure->GetName() == SavedCitizen.HomeName)
				{
					if (UHousingComponent* HousingComponent = Structure->GetComponentByClass<UHousingComponent>())
					{
						HousingComponent->AssignResident(SpawnedCitizen);
						SpawnedCitizen->AssignHome(Cast<AStructure>(Structure));
					}
				}

				if (Structure->GetName() == SavedCitizen.WorkplaceName)
				{
					if (UWorkersComponent* WorkersComponent = Structure->GetComponentByClass<UWorkersComponent>())
					{
						WorkersComponent->AssignWorker(SpawnedCitizen);
						SpawnedCitizen->AssignWorkplace(Cast<AStructure>(Structure));
					}
				}
			}
		}

		SpawnedCitizen->FinishSpawning(SavedCitizen.CitizenTransform);
		switch (SpawnedCitizen->GetCitizenState())
		{
		case ECitizenState::GoingToWork:
			Cast<ACitizenAIController>(SpawnedCitizen->GetController())->GoToWork();
			break;
		case ECitizenState::GoingHome:
			Cast<ACitizenAIController>(SpawnedCitizen->GetController())->GoToHome();
			break;
		case ECitizenState::Roaming:
			Cast<ACitizenAIController>(SpawnedCitizen->GetController())->Roam();
			break;
		case ECitizenState::AtHome:
			break;
		case ECitizenState::Working:
			break;
		default:
			break;
		}

	}
}

void UStrategyGameInstance::SaveGame()
{
	if (UStrategySaveGame* SaveGameInstance = Cast<UStrategySaveGame>(UGameplayStatics::CreateSaveGameObject(UStrategySaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;
		SavedDelegate.BindUObject(this, &ThisClass::OnGameSaved);

		SaveGameInstance->Level = GetWorld();

		// Save Player Data
		FPlayerSave PlayerSave;
		PlayerSave.PlayerTransform = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorTransform();
		SaveGameInstance->PlayerSave = PlayerSave;

		// Save Game Data
		SaveGameInstance->TimeOfDay = GetSubsystem<UTimeSubsystem>()->GetTimeOfDay();
		SaveGameInstance->DaysCityHasSurvived = GetSubsystem<UTimeSubsystem>()->GetDaysCityHasSurvived();
		SaveGameInstance->Resources = GetSubsystem<UResourcesSubsystem>()->GetResourceInventory();
		SaveGameInstance->UnlockedUpgrades = GetSubsystem<UUnlocksSubsystem>()->GetUnlockedUpgrades();

		// Save All Built Structures.
		TArray<AActor*> AllStructures;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), AllStructures);
		for (AActor* Actor : AllStructures)
		{
			// Ignores actors that were part of the level.
			if (Actor->HasAnyFlags(RF_WasLoaded)) continue;

			const AStructure* Structure = Cast<AStructure>(Actor);
			const UWorkersComponent* WorkersComponent = Structure->GetComponentByClass<UWorkersComponent>();

			FStructureSave SavedStructure;
			SavedStructure.StructureName = Structure->GetName();
			SavedStructure.StructureClass = Structure->GetClass();
			SavedStructure.StructureTransform = Structure->GetTransform();
			SavedStructure.StructureState = Structure->GetStructureState();
			SavedStructure.NumOfHiredWorkers = WorkersComponent ? WorkersComponent->GetNumOfAssignedWorkers(ECitizenType::Worker) : 0;
			SavedStructure.NumOfHiredScientists = WorkersComponent ? WorkersComponent->GetNumOfAssignedWorkers(ECitizenType::Scientist) : 0;

			UKismetSystemLibrary::LogString(SavedStructure.ToString(), true);

			SaveGameInstance->SavedStructures.Add(SavedStructure);
		}

		// Save All Citizens
		TArray<AActor*> AllCitizens;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACitizen::StaticClass(), AllCitizens);
		for (AActor* Actor : AllCitizens)
		{
			const ACitizen* Citizen = Cast<ACitizen>(Actor);

			FCitizenSave SavedCitizen;
			SavedCitizen.CitizenClass = Citizen->GetClass();
			SavedCitizen.CitizenTransform = Citizen->GetActorTransform();
			SavedCitizen.CitizenType = Citizen->GetCitizenType();
			SavedCitizen.CitizenState = Citizen->GetCitizenState();
			if (!Citizen->IsHomeless())
			{
				SavedCitizen.HomeName = Citizen->GetHome()->GetName();
			}
			else
			{
				SavedCitizen.HomeName = "";
			}

			if (Citizen->IsEmployed())
			{
				SavedCitizen.WorkplaceName = Citizen->GetWorkplace()->GetName();
			}
			else
			{
				SavedCitizen.WorkplaceName = "";
			}

			UKismetSystemLibrary::LogString(SavedCitizen.ToString(), true);

			SaveGameInstance->SavedCitizens.Add(SavedCitizen);
		}
 
		// Start async save process.
		const FString SlotNameString = "save";
		constexpr int32 UserIndexInt32 = 0;
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotNameString, UserIndexInt32, SavedDelegate);
	}
}

void UStrategyGameInstance::LoadSave()
{
	// Set up the delegate.
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	LoadedDelegate.BindUObject(this, &ThisClass::OnSaveLoaded);
	const FString SlotNameString = "save";
	constexpr int32 UserIndexInt32 = 0;
	UGameplayStatics::AsyncLoadGameFromSlot(SlotNameString, UserIndexInt32, LoadedDelegate);
}

bool UStrategyGameInstance::WasSaveLoaded() const
{
	return bWasSaveLoaded;
}
