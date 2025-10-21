// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StrategyGameState.generated.h"


UCLASS()
class STRATEGYGAME_API AStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

	AStrategyGameState();

protected:

	virtual void BeginPlay() override;
};
