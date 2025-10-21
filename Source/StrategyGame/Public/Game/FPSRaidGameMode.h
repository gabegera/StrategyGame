// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Objective.h"
#include "RaidSettings.h"
#include "FPSRaidGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectiveUpdatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllObjectivesCompletedSignature);

class ADestructible;
class AStrategyGameCharacter;
/**
 * The Game Mode that is used when you are raiding an enemy base in the First Person Mode.
 */
UCLASS()
class STRATEGYGAME_API AFPSRaidGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AFPSRaidGameMode();

protected:

	UPROPERTY(EditAnywhere)
	FRaidSettings RaidSettings;

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
	void ReactToObjectiveUpdated();

	UFUNCTION()
	void OnAllObjectivesCompleted();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnAllObjectivesCompleted")
	void ReactToAllObjectivesCompleted();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FObjectiveUpdatedSignature OnObjectiveUpdatedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAllObjectivesCompletedSignature OnAllObjectivesCompletedDelegate;

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
