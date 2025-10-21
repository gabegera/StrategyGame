// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CityDefenseGameMode.generated.h"

enum class ECitizenType : uint8;
class UStrategyGameInstance;

UCLASS()
class STRATEGYGAME_API ACityDefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ACityDefenseGameMode();

protected:

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TSet<TSubclassOf<class AStructure>> StartingStructures;

	UPROPERTY(EditAnywhere, Category="Strategy Game Mode")
	TMap<ECitizenType, int32> StartingCitizens;

	virtual void BeginPlay() override;
	
};
