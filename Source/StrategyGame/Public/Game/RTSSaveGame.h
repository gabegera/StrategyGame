// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RTSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API URTSSaveGame : public USaveGame
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
 
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
 
	URTSSaveGame();
	
};
