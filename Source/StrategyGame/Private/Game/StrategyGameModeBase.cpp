// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StrategyGameModeBase.h"

#include "Game/CityTimeSubsystem.h"

AStrategyGameModeBase::AStrategyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStrategyGameModeBase::UpdateTimeOfDay(float DeltaSeconds)
{
	UCityTimeSubsystem::IncreaseTimeOfDay(MinutesPassedPerSecond * DeltaSeconds / 60.0f);
}

void AStrategyGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateTimeOfDay(DeltaSeconds);
}	
