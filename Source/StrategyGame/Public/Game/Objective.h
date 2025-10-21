#pragma once

#include "CoreMinimal.h"
#include "ResourcesSubsystem.h"
#include "Destructible.h"
#include "Enemies/EnemyCharacter.h"
#include "Objective.generated.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	KillEnemies = 0 UMETA(DisplayName="Kill Enemies"),
	DestroyTargets = 1 UMETA(DisplayName="Destroy Targets"),
	FreePrisoners = 2 UMETA(DisplayName="Free Prisoners"),
	StealResources = 3 UMETA(DisplayName="Steal Resources")
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
		bAnyResources = true;
		TargetResource = nullptr;
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
		bAnyResources = true;
		TargetResource = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating a "Destroy Targets" Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InResourceType The type of target that needs to be destroyed to progress the objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, TSubclassOf<ADestructible> InResourceType, int32 InRequiredProgress)
	{
		Title = InTitle;
		ObjectiveType = InObjectiveType;
		EnemyType = nullptr;
		DestructibleType = InResourceType;
		bAnyResources = true;
		TargetResource = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating a "Steal Resources" Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param bInAnyResource If true, then any resource will progress this objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, bool bInAnyResource, int32 InRequiredProgress)
	{
		Title = InTitle;
		ObjectiveType = InObjectiveType;
		EnemyType = nullptr;
		DestructibleType = nullptr;
		bAnyResources = bInAnyResource;
		TargetResource = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating a "Steal Resources" Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InResourceType The type of resource that needs to be stolen to progress the objective.
	 * @param InRequiredProgress How many "Enemies Killed" are needed to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, UResourceDataAsset* InResource, int32 InRequiredProgress)
	{
		Title = InTitle;
		ObjectiveType = InObjectiveType;
		EnemyType = nullptr;
		DestructibleType = nullptr;
		bAnyResources = false;
		TargetResource = InResource;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	/**
	 * Constructor function for creating an Objective.
	 * @param InObjectiveType The type of objective.
	 * @param InTitle The new title for the objective.
	 * @param InRequiredProgress The amount of progress required to complete the objective.
	 */
	FObjective(EObjectiveType InObjectiveType, FString InTitle, int32 InRequiredProgress)
	{
		ObjectiveType = InObjectiveType;
		Title = InTitle;
		EnemyType = nullptr;
		DestructibleType = nullptr;
		bAnyResources = true;
		TargetResource = nullptr;
		Progress = 0;
		RequiredProgress = InRequiredProgress;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Title;

	// The type of enemy that needs to be killed to progress the objective.
	// If set to none, any enemy will count as objective progress.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="ObjectiveType == EObjectiveType::KillEnemies", EditConditionHides))
	TSubclassOf<AEnemyCharacter> EnemyType;

	// The type of target that needs destroyed to progress the objective.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="ObjectiveType == EObjectiveType::DestroyTargets", EditConditionHides))
	TSubclassOf<ADestructible> DestructibleType;

	// If true, stealing any resource will progress the objective. If false, a resource type will need to be specified.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="ObjectiveType == EObjectiveType::StealResources", EditConditionHides))
	bool bAnyResources;

	// The type of resource that needs stolen to progress the objective.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="ObjectiveType == EObjectiveType::StealResources && !bAnyResources", EditConditionHides))
	UResourceDataAsset* TargetResource;

	UPROPERTY(VisibleAnywhere)
	int32 Progress;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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

		case EObjectiveType::StealResources:
			return "Steal Resources";
			
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