// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CityDefenseGameMode.generated.h"

class UResourceDataAsset;
enum class ECitizenType : uint8;
class UStrategyGameInstance;

USTRUCT(BlueprintType, DisplayName="Scheduled Invasion")
struct FScheduledInvasion
{
	GENERATED_BODY()

	FScheduledInvasion()
	{
		DayOfInvasion = 0;
		HourOfInvasion = 0;
	}

	FScheduledInvasion(const int32 InDayOfInvasion, const int32 InHourOfInvasion)
	{
		DayOfInvasion = InDayOfInvasion;
		HourOfInvasion = FMath::Clamp(InHourOfInvasion, 0, 23);
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scheduled Invasion")
	int32 DayOfInvasion;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Scheduled Invasion")
	int32 HourOfInvasion;
};

UCLASS()
class STRATEGYGAME_API ACityDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACityDefenseGameMode();

protected:

	// This boolean acts as a button and whenever it's set to true all UResourceDataAsset objects will be found
	// and the StartingResources and StartingMaximumResources will be populated with them.
	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Mode")
	bool bFindAllResources;

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TMap<UResourceDataAsset*, float> StartingResources;

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TMap<UResourceDataAsset*, int32> StartingMaximumResources;

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TMap<ECitizenType, int32> StartingCitizens;

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TArray<FScheduledInvasion> ScheduledInvasions;

	virtual void BeginPlay() override;

	virtual void PostInitProperties() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void PopulateAllResources();

public:

	UFUNCTION(BlueprintCallable, Category="Strategy Game Mode")
	void SpawnCitizens(int32 NumOfWorkers, int32 NumOfScientists) const;

	UFUNCTION(BlueprintPure, Category="Strategy Game Mode")
	TMap<UResourceDataAsset*, float>& GetStartingResources();

	UFUNCTION(BlueprintPure, Category="Strategy Game Mode")
	TMap<UResourceDataAsset*, int32>& GetStartingMaxResources();
	
};
