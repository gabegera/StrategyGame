// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/FPSRaidGameMode.h"

#include "Destructible.h"
#include "Enemies/EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"

AFPSRaidGameMode::AFPSRaidGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AFPSRaidGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> EnemyCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyCharacter::StaticClass(), EnemyCharacters);

	OnObjectiveUpdatedDelegate.AddUniqueDynamic(this, &ThisClass::OnObjectiveUpdated);
	OnAllObjectivesCompletedDelegate.AddUniqueDynamic(this, &ThisClass::OnAllObjectivesCompleted);	

	for (AActor* Enemy : EnemyCharacters)
	{
		Cast<AEnemyCharacter>(Enemy)->OnCharacterDiedDelegate.AddUniqueDynamic(this, &ThisClass::OnEnemyDied);
	}

	TArray<AActor*> DestructibleActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestructible::StaticClass(), DestructibleActors);

	for (AActor* Destructible : DestructibleActors)
	{
		Cast<ADestructible>(Destructible)->OnDestructibleDestroyedDelegate.AddUniqueDynamic(this, &ThisClass::OnDestructibleDestroyed);
	}
}

void AFPSRaidGameMode::OnEnemyDied(AStrategyGameCharacter* DeadEnemy)
{
	for (FObjective& Objective : RaidObjectives)
    {
    	if (Objective.ObjectiveType == EObjectiveType::KillEnemies)
    	{
    		if (Objective.EnemyType && DeadEnemy->IsA(Objective.EnemyType))
    		{
    			Objective.Progress++;
    			OnObjectiveUpdatedDelegate.Broadcast();
    		}
		    else if (!Objective.EnemyType)
		    {
    			Objective.Progress++;
    			OnObjectiveUpdatedDelegate.Broadcast();
		    }
    	}
    }
}

void AFPSRaidGameMode::OnDestructibleDestroyed(TSubclassOf<ADestructible> DestroyedDestructibleClass)
{
	for (FObjective& Objective : RaidObjectives)
	{
		if (Objective.ObjectiveType == EObjectiveType::DestroyTargets && Objective.DestructibleType == DestroyedDestructibleClass)
		{
			Objective.Progress++;
			OnObjectiveUpdatedDelegate.Broadcast();
		}
	}
}

void AFPSRaidGameMode::OnObjectiveUpdated()
{
	ReactToObjectiveUpdated();
	
	if (IsAllObjectivesComplete())
	{
		OnAllObjectivesCompletedDelegate.Broadcast();
	}
}

void AFPSRaidGameMode::OnAllObjectivesCompleted()
{
	ReactToAllObjectivesCompleted();
}

void AFPSRaidGameMode::AddObjective(FObjective NewObjective)
{
	RaidObjectives.Add(NewObjective);
}

void AFPSRaidGameMode::AddKillEnemiesObjective(FString ObjectiveTitle, int32 EnemiesToKill, TSubclassOf<AEnemyCharacter> EnemyType)
{
	FObjective NewObjective(EObjectiveType::KillEnemies, ObjectiveTitle, EnemyType, EnemiesToKill);

	AddObjective(NewObjective);
}

void AFPSRaidGameMode::AddDestroyTargetsObjective(FString ObjectiveTitle, int32 TargetsToDestroy, TSubclassOf<ADestructible> TargetType)
{
	FObjective NewObjective(EObjectiveType::DestroyTargets, ObjectiveTitle, TargetType, TargetsToDestroy);

	AddObjective(NewObjective);
}

void AFPSRaidGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	int32 ObjectiveCounter = 0;
	for (FObjective Objective : RaidObjectives)
	{
		FString ObjectiveText = Objective.Title;
		ObjectiveText.Append(": ");
		ObjectiveText.AppendInt(Objective.Progress);
		ObjectiveText.Append(" / ");
		ObjectiveText.AppendInt(Objective.RequiredProgress);
		
		GEngine->AddOnScreenDebugMessage(900 + ObjectiveCounter, DeltaSeconds, Objective.IsComplete() ? FColor::Green : FColor::Red, ObjectiveText);

		ObjectiveCounter++;
	}
}

bool AFPSRaidGameMode::IsAllObjectivesComplete()
{
	for (FObjective Objective : RaidObjectives)
	{		
		if (!Objective.IsComplete())
		{
			return false;
		}
	}

	return true;
}
