// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StrategyGameState.generated.h"

class AStrategyGameModeBase;
class AStructure;
class ARoad;

UENUM(BlueprintType, DisplayName="Time Scale")
enum class ETimeScale : uint8
{
	OneTimesSpeed		UMETA(DisplayName="1x Speed"),
	TwoTimesSpeed		UMETA(DisplayName="2x Speed"),
	ThreeTimesSpeed		UMETA(DisplayName="3x Speed"),
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeScaleChangedDelegate, ETimeScale, NewTimeScale);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureBuiltDelegate, AStructure*, BuiltStructure);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkyscraperModuleAddedDelegate, ASkyscraper*, Skyscraper, ASkyscraperModule*, AddedModule);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureDestroyedDelegate, AStructure*, DestroyedStructure);

UCLASS()
class STRATEGYGAME_API AStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

	AStrategyGameState();

protected:

	UPROPERTY()
	AStrategyGameModeBase* StrategyGameMode = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Time")
	ETimeScale TimeScale = ETimeScale::OneTimesSpeed;

	virtual void BeginPlay() override;

	UFUNCTION() void OnStructureBuilt(AStructure* BuiltStructure);
	UFUNCTION() void OnStructureDestroyed(AStructure* BuiltStructure);

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FTimeScaleChangedDelegate OnTimeScaleChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStructureBuiltDelegate StructureBuiltDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FSkyscraperModuleAddedDelegate OnSkyscraperModuleAdded;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FStructureBuiltDelegate StructureDestroyedDelegate;

	// Finds all the structures that have been placed and returns an array.
	UFUNCTION()
	TArray<AStructure*> FindAllStructures();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameModeBase* GetStrategyGameMode();

	UFUNCTION(BlueprintCallable, Category="Time")
	ETimeScale SetTimeScale(ETimeScale NewTimeScale);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Time")
	ETimeScale GetTimeScale() { return TimeScale; }

	
};
