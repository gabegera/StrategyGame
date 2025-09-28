// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CityTimeSubsystem.h"
#include "GameFramework/SaveGame.h"
#include "RTSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API URTSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	URTSSaveGame();

	UPROPERTY(VisibleAnywhere, Category="Save File")
	FString SaveSlotName;
 
	UPROPERTY(VisibleAnywhere, Category="Save File")
	float TimeOfDay;

	UPROPERTY(VisibleAnywhere, Category="Save File")
	int32 DaysCityHasSurvived;
	
};
