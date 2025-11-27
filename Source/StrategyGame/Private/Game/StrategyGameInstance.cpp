// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StrategyGameInstance.h"

#include "StrategyEnums.h"
#include "Building/Structure.h"
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

	GetSubsystem<UTimeSubsystem>()->SetTimeOfDay(LoadedSave->TimeOfDay);
	GetSubsystem<UTimeSubsystem>()->SetDaysCityHasSurvived(LoadedSave->DaysCityHasSurvived);
	GetSubsystem<UResourcesSubsystem>()->SetResourceInventory(LoadedSave->Resources);
	GetSubsystem<UUnlocksSubsystem>()->SetUnlockedUpgrades(LoadedSave->UnlockedUpgrades);

	UKismetSystemLibrary::PrintString(GetWorld(), "Save Loaded");
}

void UStrategyGameInstance::LoadSavedStructures(TArray<FStructureSave> SavedStructures)
{
	for (FStructureSave SavedStructure : SavedStructures)
	{
		if (!SavedStructure.StructureClass) continue;

		AStructure* SpawnedStructure = GetWorld()->SpawnActorDeferred<AStructure>(SavedStructure.StructureClass, SavedStructure.StructureTransform);
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
			SavedStructure.StructureClass = Structure->GetClass();
			SavedStructure.StructureTransform = Structure->GetTransform();
			SavedStructure.StructureState = Structure->GetStructureState();
			SavedStructure.NumOfHiredWorkers = WorkersComponent ? WorkersComponent->GetNumOfAssignedWorkers(ECitizenType::Worker) : 0;
			SavedStructure.NumOfHiredScientists = WorkersComponent ? WorkersComponent->GetNumOfAssignedWorkers(ECitizenType::Scientist) : 0;

			UKismetSystemLibrary::PrintString(GetWorld(), SavedStructure.ToString(), true, true, FLinearColor::Green, 5.0f);

			SaveGameInstance->SavedStructures.Add(SavedStructure);
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
