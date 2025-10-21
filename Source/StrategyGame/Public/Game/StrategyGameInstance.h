// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "StrategyGameInstance.generated.h"

class AStructure;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureBuiltSignature, AStructure*, NewStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStructureDestroyedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureSelectedSignature, AStructure*, SelectedStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStructureDeSelectedSignature);

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UStrategyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Controls the size of the snapping grid for structures.
	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Instance")
	uint16 GridSize = 500;

	UPROPERTY(EditDefaultsOnly, Category="Strategy Game Instance")
	TSet<TSubclassOf<AStructure>> StartingStructures;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureBuiltSignature OnStructureBuilt;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureDestroyedSignature OnStructureDestroyed;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureSelectedSignature OnStructureSelected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureDeSelectedSignature OnStructureDeSelected;

	UFUNCTION(BlueprintPure, Category="Strategy Game Instance")
	int32 GetGridSize() const { return GridSize; }
};
