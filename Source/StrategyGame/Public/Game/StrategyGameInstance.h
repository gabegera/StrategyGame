// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StrategyGameInstance.generated.h"

struct FPlayerSave;
struct FResourceNodeSave;
class UStrategySaveGame;
struct FCitizenSave;
class UWorkersComponent;
class UHousingComponent;
struct FStructureSave;
class USaveGame;
enum class ECitizenType : uint8;
class AStructure;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureBuiltSignature, AStructure*, NewStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureDestroyedSignature, AStructure*, DestroyedStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureSelectedSignature, AStructure*, SelectedStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStructureDeSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FResidentRequestedSignature, AStructure*, Home, UHousingComponent*, Housing);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWorkerRequestedSignature, AStructure*, Workplace, UWorkersComponent*, WorkersComponent, ECitizenType, WorkerType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGameSavedSignature, const FString, SlotName, const int32, UserIndex, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSaveLoadedSignature, const FString, SlotName, const int32, UserIndex, USaveGame*, LoadedGameData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGamePausedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameUnPausedSignature);

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UStrategyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Controls the size of the snapping grid for structures.
	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Instance")
	uint16 GridSize = 500;

	// Used to control functions that are performed when a new game is started.
	UPROPERTY()
	bool bWasSaveLoaded = false;

	UFUNCTION()
	virtual void OnGameSaved(const FString& SlotName, const int32 UserIndex, bool bSuccess);

	UFUNCTION()
	virtual void OnSaveLoaded(const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData);

	UFUNCTION()
	void LoadSavedStructures(const TArray<FStructureSave> SavedStructures);

	UFUNCTION()
	void LoadSavedCitizens(const TArray<FCitizenSave> SavedCitizens);

	UFUNCTION()
	void LoadSavedResourceNodes(const TArray<FResourceNodeSave> SavedResources);

	UFUNCTION()
	void LoadPlayer(const FPlayerSave PlayerSave);

	UFUNCTION()
	void BroadcastFinishedLoading(const FString SlotName, const int32 UserIndex, USaveGame* LoadedGameData) const;

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureBuiltSignature OnStructureBuilt;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureDestroyedSignature OnStructureDestroyed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureSelectedSignature OnStructureSelected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureDeSelectedSignature OnStructureDeSelected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FResidentRequestedSignature OnResidentRequested;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWorkerRequestedSignature OnWorkerRequested;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGameSavedSignature OnGameFinishedSaving;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FSaveLoadedSignature OnFinishedLoadingSave;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGamePausedSignature OnGamePaused;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FGameUnPausedSignature OnGameUnPaused;

	UFUNCTION(BlueprintPure, Category="Strategy Game Instance")
	int32 GetGridSize() const { return GridSize; }

	UFUNCTION(BlueprintCallable, Category="Strategy Game Instance")
	void SaveGame();

	UFUNCTION(BlueprintCallable, Category="Strategy Game Instance")
	void LoadSave();

	// Used to control functions that are performed when a new game is started.
	UFUNCTION(BlueprintCallable, Category="Strategy Game Instance")
	bool WasSaveLoaded() const;
};
