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

void UTimeSubsystem::IncreaseTimeOfDay(const float InHours)
{
	TimeOfDay += InHours * TimeScaleMultiplier;
	
	if (TimeOfDay >= 24.0f)
	{
		TimeOfDay -= 24.0f;
		DaysCitySurvived++;
	}

	OnTimePassed.Broadcast(InHours);
}

void UTimeSubsystem::SetTimeScaleMultiplier(const float InMultiplier)
{
	TimeScaleMultiplier = InMultiplier;
	OnTimeScaleChanged.Broadcast(TimeScaleMultiplier);
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

bool UTimeSubsystem::IsWorkTime() const
{
	return TimeOfDay < WorkEndTime && TimeOfDay > WorkStartTime;
}
