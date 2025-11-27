// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StrategyStatics.generated.h"

class AStructure;
class UResourceDataAsset;
enum class ECitizenType : uint8;

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UStrategyStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="Game", meta=(WorldContext="WorldContextObject", UnsafeDuringActorConstruction="true"))
	static int32 GetGridSize(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Game")
	static TArray<UResourceDataAsset*> GetAllResources();

	UFUNCTION(BlueprintPure, Category="Game")
	static UResourceDataAsset* GetResourceByName(const FString& InName);

	UFUNCTION(BlueprintPure, Category="Game")
	static TSet<TSubclassOf<AStructure>> GetAllStructureClasses();
};
