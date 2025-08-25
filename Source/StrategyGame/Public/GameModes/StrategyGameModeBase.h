// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "StrategyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API AStrategyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	// Controls the size of the snapping grid for structures.
	UPROPERTY(EditAnywhere, Category="Structure Building")
	int32 SnappingSize = 500;

public:

	// Gets the size of the snapping grid for structures.
	UFUNCTION(BlueprintGetter)
	int32 GetSnappingSize() { return SnappingSize; }
	
};
