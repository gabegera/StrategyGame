// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomActor.h"


// Sets default values
ACustomActor::ACustomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

AStrategyGameState* ACustomActor::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

AStrategyGameModeBase* ACustomActor::GetStrategyGameMode()
{
	if (StrategyGameMode == nullptr)
	{
		StrategyGameMode = Cast<AStrategyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	return StrategyGameMode;
}

