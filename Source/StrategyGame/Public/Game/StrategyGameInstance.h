// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StrategyGameInstance.generated.h"

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
	virtual void LoadSavedStructures(const TArray<FStructureSave> SavedStructures);

	UFUNCTION()
	virtual void LoadSavedCitizens(const TArray<FCitizenSave> SavedCitizens);

	UFUNCTION()
	virtual void LoadSavedResourceNodes(const TArray<FResourceNodeSave> SavedResources);

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
