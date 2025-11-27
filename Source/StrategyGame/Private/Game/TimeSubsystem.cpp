// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TimeSubsystem.h"

UTimeSubsystem::UTimeSubsystem()
{
	
}

void UTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FTimerDelegate UpdateTimeOfDayTimerDelegate;
	UpdateTimeOfDayTimerDelegate.BindUObject(this, &ThisClass::IncreaseTimeOfDay, UpdateTimeOfDayFrequency * MinutesPassedPerSecond / 60.0f);
	constexpr bool bShouldLoop = true;
	GetWorld()->GetTimerManager().SetTimer(UpdateTimeOfDayTimer, UpdateTimeOfDayTimerDelegate, UpdateTimeOfDayFrequency, bShouldLoop);
}

float UTimeSubsystem::SetTimeOfDay(float InHours)
{
	while (InHours >= 24.0f)
	{
		InHours -= 24.0f;
	}
	return TimeOfDay = InHours;
}

int32 UTimeSubsystem::SetDaysCityHasSurvived(const int32 InDays)
{
	return DaysCitySurvived = InDays;
}

void UTimeSubsystem::IncreaseTimeOfDay(const float InHours)
{
	const bool bWasWorkTime = IsTimeToWork();
	
	TimeOfDay += InHours * TimeScaleMultiplier;
	
	while (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
		DaysCitySurvived++;
	}

	if (IsTimeToWork() && !bWasWorkTime)
    {
    	OnWorkTimeStarted.Broadcast();
    }
	else if (!IsTimeToWork() && bWasWorkTime)
	{
		OnWorkTimeEnded.Broadcast();
	}

	OnTimePassed.Broadcast(InHours);
}

void UTimeSubsystem::SetTimeScaleMultiplier(const float InMultiplier)
{
	TimeScaleMultiplier = InMultiplier;
	OnTimeScaleChanged.Broadcast(TimeScaleMultiplier);
}

float UTimeSubsystem::GetTimeOfDay() const
{
	return TimeOfDay;
}

FString UTimeSubsystem::GetTimeOfDayString()
{
	return GetTimeOfDayHoursString() + " : " + GetTimeOfDayMinutesString();
}

int32 UTimeSubsystem::GetTimeOfDayHours()
{
	return FMath::FloorToInt32(TimeOfDay);
}

FString UTimeSubsystem::GetTimeOfDayHoursString()
{
	FString OutString = FString::FromInt(GetTimeOfDayHours());

	if (OutString.Len() < 2)
	{
		OutString.InsertAt(0, '0');
	}

	return OutString;
}

int32 UTimeSubsystem::GetTimeOfDayMinutes()
{
	return FMath::FloorToInt32((TimeOfDay - GetTimeOfDayHours()) * 60.0f);
}

FString UTimeSubsystem::GetTimeOfDayMinutesString()
{
	FString OutString = FString::FromInt(GetTimeOfDayMinutes());

	if (OutString.Len() < 2)
	{
		OutString.InsertAt(0, '0');
	}

	return OutString;
}

float UTimeSubsystem::GetWorkStartTime() const
{
	return WorkStartTime;
}

float UTimeSubsystem::GetWorkEndTime() const
{
	return WorkEndTime;
}

bool UTimeSubsystem::IsTimeToWork() const
{
	return TimeOfDay < WorkEndTime && TimeOfDay > WorkStartTime;
}