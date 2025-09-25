#pragma once

#include "CoreMinimal.h"
#include "Destructible.h"
#include "Enemies/EnemyCharacter.h"
#include "Objective.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	KillEnemies = 0 UMETA(DisplayName="Kill Enemies"),
	DestroyTargets = 1 UMETA(DisplayName="Destroy Targets"),
	FreePrisoners = 2 UMETA(DisplayName="Free Prisoners")
};

USTRUCT(BlueprintType)
struct FObjective
{

	GENERATED_BODY()

	FObjective()
	{
		ObjectiveType = EObjectiveType::KillEnemies;
		Title = "Unnamed Objective";
		EnemyType = nullptr;
		DestructibleType = nullptr;
		Progress = 0;
		RequiredProgress = 1;
	}

	/**
	 * Constructor function for creating a "Kill Enemies" Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InEnemyType The type of enemy that needs to be killed to progress the objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, TSubclassOf<AEnemyCharacter> InEnemyType, int32 InRequiredProgress)
	{
		Title = InTitle;
		ObjectiveType = InObjectiveType;
		EnemyType = InEnemyType;
		DestructibleType = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating a "Destroy Targets" Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InDestructibleType The type of target that needs to be destroyed to progress the objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, TSubclassOf<ADestructible> InDestructibleType, int32 InRequiredProgress)
	{
		Title = InTitle;
		ObjectiveType = InObjectiveType;
		EnemyType = nullptr;
		DestructibleType = InDestructibleType;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating an Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, int32 InRequiredProgress)
	{
		ObjectiveType = InObjectiveType;
		Title = InTitle;
		EnemyType = nullptr;
		DestructibleType = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	UPROPERTY(EditAnywhere)
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere)
	FString Title;

	// The type of enemy that needs to be killed to progress the objective.
	// If set to none, any enemy will count as objective progress.
	UPROPERTY(EditAnywhere, meta=(EditCondition="ObjectiveType == EObjectiveType::KillEnemies", EditConditionHides))
	TSubclassOf<AEnemyCharacter> EnemyType;

	// The type of target that needs destroyed to progress the objective.
	UPROPERTY(EditAnywhere, meta=(EditCondition="ObjectiveType == EObjectiveType::DestroyTargets", EditConditionHides))
	TSubclassOf<ADestructible> DestructibleType;

	UPROPERTY(VisibleAnywhere)
	int32 Progress;

	UPROPERTY(EditAnywhere)
	int32 RequiredProgress;

	bool IsComplete() const
	{
		return Progress >= RequiredProgress;
	}

	// Returns a string for the Objective Type.
	FString GetTypeString() const
	{
		switch (ObjectiveType)
		{
		case EObjectiveType::KillEnemies:
			return "Kill Enemies";
			
		case EObjectiveType::DestroyTargets:
			return "Destroy Targets";
			
		case EObjectiveType::FreePrisoners:
			return "Free Prisoners";
			
		default:
			return "Empty Objective Type";
		}
	}

	bool operator==(const FObjective& Other) const
	{
		return	Title == Other.Title &&
				ObjectiveType == Other.ObjectiveType &&
				EnemyType == Other.EnemyType &&
				DestructibleType == Other.DestructibleType &&
				RequiredProgress == Other.RequiredProgress;
	}
};