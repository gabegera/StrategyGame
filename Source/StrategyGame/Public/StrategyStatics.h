// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StrategyStatics.generated.h"

enum class ECitizenType : uint8;
/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UStrategyStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

protected:
	
	static inline TSubclassOf<class ACitizen> CitizenClassToSpawn;

public:

	UStrategyStatics();

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static int32 GetGridSize(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="Civilians", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SpawnCitizen(const UObject* WorldContextObject, ECitizenType CitizenType);

	UFUNCTION(BlueprintCallable, Category="Civilians", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static void SpawnCitizensMulti(const UObject* WorldContextObject, int32 NumOfWorkers, int32 NumOfScientists);

	
};
