// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game/CityDefenseGameMode.h"
#include "Game/StrategyGameState.h"
#include "CustomActor.generated.h"

UCLASS()
class STRATEGYGAME_API ACustomActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomActor();

protected:

	virtual void BeginPlay() override;

public:
};
