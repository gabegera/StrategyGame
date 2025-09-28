// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CityTimeSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UCityTimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UCityTimeSubsystem();

protected:

	// The Time of Day in hours. Ranges between 0 and 24
	static inline float TimeOfDay = 12.0f;

	// Number of days the city has survived.
	static inline int32 DaysCitySurvived = 0;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	
	UFUNCTION(BlueprintCallable, Category="Time")
	static void IncreaseTimeOfDay(float InHours);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static float GetTimeOfDay() { return TimeOfDay; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static int32 GetTimeOfDayHours();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static FString GetTimeOfDayHoursString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static int32 GetTimeOfDayMinutes();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static FString GetTimeOfDayMinutesString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	static int32 GetDaysCityHasSurvived() { return DaysCitySurvived; }
};
