// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TimeSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimePassedSignature, float, HoursPassed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeScaleChangedSignature, float, NewTimeScaleMultipier);

/**
 * 
 */
UCLASS(DisplayName="Time Subsystem")
class STRATEGYGAME_API UTimeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UTimeSubsystem();

protected:
	
	FTimerHandle UpdateTimeOfDayTimer;

	// Time in seconds between each time update.
	UPROPERTY(EditAnywhere, Category="Time Subsystem", meta=(Units="Seconds"))
	float UpdateTimeOfDayFrequency = 0.2f;
	
	// How many minutes pass each real-life second.
	UPROPERTY(EditAnywhere, Category="Time Subsystem", meta=(Units="Minutes"))
	float MinutesPassedPerSecond = 5.0f;

	UPROPERTY(VisibleAnywhere, Category="Time Subsystem")
	float TimeScaleMultiplier = 1.0f;

	// The Time of Day in hours. Ranges between 0 and 24
	UPROPERTY(VisibleAnywhere, Category="Time Subsystem", meta=(Units="Hours"))
	float TimeOfDay = 12.0f;

	UPROPERTY(VisibleAnywhere, Category="Time Subsystem", meta=(Units="Days"))
	int32 DaysCitySurvived = 0;

	// At what time of day the citizens will start their work day.
	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Instance", meta=(ClampMin=0.0f, ClampMax=23.5))
	float WorkStartTime = 8.0f;

	// At what time of day the citizens will end their work day and go home.
	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Instance", meta=(ClampMin=0.0f, ClampMax=23.5))
	float WorkEndTime = 15.0f;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Time Subsystem")
	FTimePassedSignature OnTimePassed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="Time Subsystem")
	FTimeScaleChangedSignature OnTimeScaleChanged;
	
	UFUNCTION(BlueprintCallable, Category="Time Subsystem")
	void IncreaseTimeOfDay(float InHours);

	UFUNCTION(BlueprintCallable, Category="Time Subsystem")
	void SetTimeScaleMultiplier(const float InMultiplier);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	float GetTimeOfDay() const { return TimeOfDay; }

	// Gets the time of day as a string in a 00:00 format.
	UFUNCTION(BlueprintPure, Category="Time Subsystem")
	FString GetTimeOfDayString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	int32 GetTimeOfDayHours();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	FString GetTimeOfDayHoursString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	int32 GetTimeOfDayMinutes();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	FString GetTimeOfDayMinutesString();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time Subsystem")
	int32 GetDaysCityHasSurvived() const { return DaysCitySurvived; }

	UFUNCTION(BlueprintPure, Category="Time Subsystem")
	float GetWorkStartTime() const;

	UFUNCTION(BlueprintPure, Category="Time Subsystem")
	float GetWorkEndTime() const;

	UFUNCTION(BlueprintPure, Category="Time Subsystem")
	bool IsWorkTime() const;
};
