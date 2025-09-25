// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyGameModeBase.h"
#include "Objective.h"
#include "FPSRaidGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveUpdatedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObjectivesCompletedDelegate);

class ADestructible;
class AStrategyGameCharacter;
/**
 * The Game Mode that is used when you are raiding an enemy base in the First Person Mode.
 */
UCLASS()
class STRATEGYGAME_API AFPSRaidGameMode : public AStrategyGameModeBase
{
	GENERATED_BODY()

public:
	
	AFPSRaidGameMode();

protected:

	UPROPERTY(VisibleAnywhere)
	TArray<FObjective> RaidObjectives;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEnemyDied(AStrategyGameCharacter* DeadEnemy);

	UFUNCTION()
	virtual void OnDestructibleDestroyed(TSubclassOf<ADestructible> DestroyedDestructibleClass);

	UFUNCTION()
	virtual void OnObjectiveUpdated();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnObjectiveUpdated")
	void BP_OnObjectiveUpdated();

	UFUNCTION()
	void OnAllObjectivesCompleted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnAllObjectivesCompleted")
	void BP_OnAllObjectivesCompleted();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnObjectiveUpdatedDelegate OnObjectiveUpdatedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAllObjectivesCompletedDelegate OnAllObjectivesCompletedDelegate;

	UFUNCTION(BlueprintCallable, Category="FPS Raid Game Mode")
	void AddObjective(FObjective NewObjective);

	UFUNCTION(BlueprintCallable, Category="FPS Raid Game Mode")
	void AddKillEnemiesObjective(FString ObjectiveTitle, int32 EnemiesToKill, TSubclassOf<AEnemyCharacter> EnemyType = nullptr);

	UFUNCTION(BlueprintCallable, Category="FPS Raid Game Mode")
	void AddDestroyTargetsObjective(FString ObjectiveTitle, int32 TargetsToDestroy, TSubclassOf<ADestructible> TargetType = nullptr);

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAllObjectivesComplete();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FObjective>& GetRaidObjectives() { return RaidObjectives; }
	
};
