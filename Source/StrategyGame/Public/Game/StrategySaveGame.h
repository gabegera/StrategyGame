// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Structure.h"
#include "Citizens/Citizen.h"
#include "GameFramework/SaveGame.h"
#include "StrategySaveGame.generated.h"

enum class ECitizenState : uint8;
class UUpgradeDataAsset;
class UResourceDataAsset;

USTRUCT(BlueprintType)
struct FPlayerSave
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform PlayerTransform;
};

USTRUCT(BlueprintType)
struct FStructureSave
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AStructure> StructureClass;

	UPROPERTY()
	FString StructureName;

	UPROPERTY()
	FTransform StructureTransform;

	UPROPERTY()
	EStructureState StructureState;

	UPROPERTY()
	int32 NumOfHiredWorkers;

	UPROPERTY()
	int32 NumOfHiredScientists;

	FString ToString() const
	{
		FString OutString = "Class: ";
		OutString.Append(StructureClass->GetName());
		OutString.Append(", StructureTransform: ");
		OutString.Append(StructureTransform.ToString());
		OutString.Append(", Structure State: ");
		OutString.Append(UEnum::GetDisplayValueAsText(StructureState).ToString());

		return OutString;
	}
};

USTRUCT(BlueprintType)
struct FCitizenSave
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<ACitizen> CitizenClass;

	UPROPERTY()
	FTransform CitizenTransform;

	UPROPERTY()
	ECitizenType CitizenType;

	UPROPERTY()
	ECitizenState CitizenState;

	UPROPERTY()
	FString HomeName;

	UPROPERTY()
	FString WorkplaceName;

	FString ToString() const
	{
		FString OutString = "Class: ";
		OutString.Append(CitizenClass->GetName());
		OutString.Append(", StructureTransform: ");
		OutString.Append(CitizenTransform.ToString());
		OutString.Append(", Structure State: ");
		OutString.Append(UEnum::GetDisplayValueAsText(CitizenType).ToString());

		return OutString;
	}
};


/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UStrategySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UStrategySaveGame();

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	TSoftObjectPtr<UWorld> Level;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	FPlayerSave PlayerSave;
 
	UPROPERTY(BlueprintReadOnly, Category="Save File")
	float TimeOfDay;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	int32 DaysCityHasSurvived;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	TArray<FStructureSave> SavedStructures;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	TMap<UResourceDataAsset*, float> Resources;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	TSet<UUpgradeDataAsset*> UnlockedUpgrades;

	UPROPERTY(BlueprintReadOnly, Category="Save File")
	TArray<FCitizenSave> SavedCitizens;
};
