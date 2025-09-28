// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/CityTimeSubsystem.h"

UCityTimeSubsystem::UCityTimeSubsystem()
{
	
}

void UCityTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCityTimeSubsystem::IncreaseTimeOfDay(float InHours)
{
	TimeOfDay += InHours;
	
	if (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
		DaysCitySurvived++;
	}
}

int32 UCityTimeSubsystem::GetTimeOfDayHours()
{
	return FMath::FloorToInt32(TimeOfDay);
}

FString UCityTimeSubsystem::GetTimeOfDayHoursString()
{
	FString OutString = FString::FromInt(GetTimeOfDayHours());

	if (OutString.Len() < 2)
	{
		OutString.InsertAt(0, '0');
	}

	return OutString;
}

int32 UCityTimeSubsystem::GetTimeOfDayMinutes()
{
	return FMath::FloorToInt32((TimeOfDay - GetTimeOfDayHours()) * 60.0f);
}

FString UCityTimeSubsystem::GetTimeOfDayMinutesString()
{
	FString OutString = FString::FromInt(GetTimeOfDayMinutes());

	if (OutString.Len() < 2)
	{
		OutString.InsertAt(0, '0');
	}

	return OutString;
}
