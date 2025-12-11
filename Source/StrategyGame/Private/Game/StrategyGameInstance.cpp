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
#include "Player/RTSCamera.h"

void UStrategyGameInstance::OnGameSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Game Saved");

	OnGameFinishedSaving.Broadcast(SlotName, UserIndex, bSuccess);
}

void UStrategyGameInstance::OnSaveLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData)
{
	const UStrategySaveGame* LoadedSave = Cast<UStrategySaveGame>(LoadedGameData);

	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LoadedSave->Level);

	GetSubsystem<UTimeSubsystem>()->SetTimeOfDay(LoadedSave->TimeOfDay);
    GetSubsystem<UTimeSubsystem>()->SetDaysCityHasSurvived(LoadedSave->DaysCityHasSurvived);
    GetSubsystem<UResourcesSubsystem>()->SetResourceInventory(LoadedSave->CityResources);
    GetSubsystem<UUnlocksSubsystem>()->SetUnlockedUpgrades(LoadedSave->UnlockedUpgrades);

	// Physical Actors need to be loaded on the next tick.
	FTimerDelegate StructureLoadingDelegate;
	StructureLoadingDelegate.BindUObject(this, &ThisClass::LoadSavedStructures, LoadedSave->SavedStructures);
	GetTimerManager().SetTimerForNextTick(StructureLoadingDelegate);

	FTimerDelegate CitizenLoadingDelegate;
	CitizenLoadingDelegate.BindUObject(this, &ThisClass::LoadSavedCitizens, LoadedSave->SavedCitizens);
	GetTimerManager().SetTimerForNextTick(CitizenLoadingDelegate);

	FTimerDelegate ResourcesLoadingDelegate;
	ResourcesLoadingDelegate.BindUObject(this, &ThisClass::LoadSavedResourceNodes, LoadedSave->SavedResourceNodes);
	GetTimerManager().SetTimerForNextTick(ResourcesLoadingDelegate);

	FTimerDelegate PlayerLoadingDelegate;
	PlayerLoadingDelegate.BindUObject(this, &ThisClass::LoadPlayer, LoadedSave->PlayerSave);
	GetTimerManager().SetTimerForNextTick(PlayerLoadingDelegate);

	// Keep this Timer and Delegate at the end. Broadcasts a delegate notifying that everything else has finished loading.
	FTimerDelegate FinishedLoadingDelegate;
	FinishedLoadingDelegate.BindUObject(this, &ThisClass::BroadcastFinishedLoading, SlotName, UserIndex, LoadedGameData);
	GetTimerManager().SetTimerForNextTick(FinishedLoadingDelegate);

	bWasSaveLoaded = true;
}

void UStrategyGameInstance::LoadSavedStructures(const TArray<FStructureSave> SavedStructures)
{
	for (FStructureSave SavedStructure : SavedStructures)
	{
		checkf(SavedStructure.StructureClass, TEXT("The structure class %s failed to load from save file."), *SavedStructure.StructureClass->GetName());

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
	// Spawns all saved citizens.
	for (FCitizenSave SavedCitizen : SavedCitizens)
	{
		checkf(SavedCitizen.CitizenClass, TEXT("The citizen class %s failed to load from save file."), *SavedCitizen.CitizenClass->GetName());

		ACitizen* SpawnedCitizen = GetWorld()->SpawnActorDeferred<ACitizen>(SavedCitizen.CitizenClass, FTransform());
		SpawnedCitizen->SetCitizenType(SavedCitizen.CitizenType);
		SpawnedCitizen->SetCitizenState(SavedCitizen.CitizenState);

		// If the citizen has either a home or a workplace, finds them and assigns them.
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

						if (SpawnedCitizen->IsEmployed() || SavedCitizen.WorkplaceName == "") break; // Early breaks to prevent iterating for longer than needed.
					}
				}

				if (Structure->GetName() == SavedCitizen.WorkplaceName)
				{
					if (UWorkersComponent* WorkersComponent = Structure->GetComponentByClass<UWorkersComponent>())
					{
						constexpr bool bForceSuccess = true;
						WorkersComponent->AssignWorker(SpawnedCitizen, bForceSuccess);
						SpawnedCitizen->AssignWorkplace(Cast<AStructure>(Structure));
						if (!SpawnedCitizen->IsHomeless() || SavedCitizen.HomeName == "") break; // Early breaks to prevent iterating for longer than needed.
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
			SpawnedCitizen->EnterStructure(SpawnedCitizen->GetHome());
			break;
		case ECitizenState::Working:
			SpawnedCitizen->EnterStructure(SpawnedCitizen->GetWorkplace());
			break;
		default:
			break;
		}
	}
}

void UStrategyGameInstance::LoadSavedResourceNodes(const TArray<FResourceNodeSave> SavedResources)
{
	// Destroy all pre-existing resources to make room the loaded ones.
	TArray<AActor*> AllResources;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResourceNode::StaticClass(), AllResources);
	for (AActor* Resource : AllResources)
	{
		if (Resource)
		{
			Resource->Destroy();
		}
	}

	// Spawn all saved resources.
	for (FResourceNodeSave Resource : SavedResources)
	{
		checkf(Resource.ResourceClass, TEXT("The resource class %s failed to load from save file."), *Resource.ResourceClass->GetName());

		AResourceNode* SpawnedResource = GetWorld()->SpawnActorDeferred<AResourceNode>(Resource.ResourceClass, FTransform());
		SpawnedResource->SetResourceAmount(Resource.ResourceAmount);
		SpawnedResource->FinishSpawning(Resource.ResourceTransform);
	}
}

void UStrategyGameInstance::LoadPlayer(const FPlayerSave PlayerSave)
{
	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorTransform(PlayerSave.PlayerTransform);
	ARTSCamera* RTSCamera = Cast<ARTSCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ARTSCamera::StaticClass()));
	RTSCamera->SetActorTransform(PlayerSave.RTSCameraTransform);
	RTSCamera->SetZoom(PlayerSave.RTSCameraZoom);
}

void UStrategyGameInstance::BroadcastFinishedLoading(const FString SlotName, const int32 UserIndex, USaveGame* LoadedGameData) const
{
	OnFinishedLoadingSave.Broadcast(SlotName, UserIndex, LoadedGameData);
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
		ARTSCamera* RTSCamera = Cast<ARTSCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), ARTSCamera::StaticClass()));
		PlayerSave.RTSCameraTransform = RTSCamera->GetTransform();
		PlayerSave.RTSCameraZoom = RTSCamera->GetZoom();
		SaveGameInstance->PlayerSave = PlayerSave;

		// Save Game Data
		SaveGameInstance->TimeOfDay = GetSubsystem<UTimeSubsystem>()->GetTimeOfDay();
		SaveGameInstance->DaysCityHasSurvived = GetSubsystem<UTimeSubsystem>()->GetDaysCityHasSurvived();
		SaveGameInstance->CityResources = GetSubsystem<UResourcesSubsystem>()->GetResourceInventory();
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

		// Save All Resource Nodes
		TArray<AActor*> AllResources;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AResourceNode::StaticClass(), AllResources);
		for (AActor* Actor : AllResources)
		{
			const AResourceNode* Resource  = Cast<AResourceNode>(Actor);

			FResourceNodeSave SavedResource;
			SavedResource.ResourceClass = Resource->GetClass();
			SavedResource.ResourceTransform = Resource->GetTransform();
			SavedResource.ResourceAmount = Resource->GetResourceAmount();

			UKismetSystemLibrary::LogString(SavedResource.ToString(), true);

			SaveGameInstance->SavedResourceNodes.Add(SavedResource);
		}

		// Start async save process.
		const FString SlotNameString = "save";
		constexpr int32 UserIndexInt32 = 0;
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotNameString, UserIndexInt32, SavedDelegate);
	}
}

void UStrategyGameInstance::LoadSave()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &ThisClass::OnSaveLoaded);
	const FString SlotNameString = "save";
	constexpr int32 UserIndexInt32 = 0;
	UGameplayStatics::AsyncLoadGameFromSlot(SlotNameString, UserIndexInt32, LoadedDelegate);
}

bool UStrategyGameInstance::WasSaveLoaded() const
{
	return bWasSaveLoaded;
}
