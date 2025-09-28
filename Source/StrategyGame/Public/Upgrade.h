#pragma once

#include "CoreMinimal.h"
#include "Upgrade.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	UnlockStructure = 0						UMETA(DisplayName="Unlock Structure"),
	ChangeStructureStorageCapacity = 1		UMETA(DisplayName="Change Structure Storage Capacity"),
	ChangeStructureResourceGeneration = 2	UMETA(DisplayName="Change Structure Resource Generation"),
	ChangeStructureResourceDrain = 3		UMETA(DisplayName="Change Structure Resource Drain"),
	UnlockPlayerEquipment = 4				UMETA(DisplayName="Unlock Player Equipment"),
};

USTRUCT(BlueprintType)
struct FUpgrade
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUpgradeType UpgradeType;

	// Either a new structure being unlocked or the structure that is getting an upgrade.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="UpgradeType != EUpgradeType::UnlockPlayerEquipment", EditConditionHides))
	TSoftClassPtr<class ABuildable> TargetStructure;
	 
	// Determines how much the target structure will either Change Storage Capacity, Change Resource Generation, or Change Resource Draining. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="UpgradeType != EUpgradeType::UnlockPlayerEquipment && UpgradeType != EUpgradeType::UnlockStructure", EditConditionHides))
	int32 StructureValueChange;

	// Either a new equippable being unlocked or the equippable that is getting an upgrade.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="UpgradeType == EUpgradeType::UnlockPlayerEquipment", EditConditionHides))
	TSoftClassPtr<class AEquippableItem> TargetEquippable;

	// Returns a string for the Upgrade Type.
	FString GetTypeString() const
	{
		switch (UpgradeType)
		{
		case EUpgradeType::UnlockStructure:
			return "Unlock Structure";
			
		case EUpgradeType::ChangeStructureStorageCapacity:
			return "Change Structure Storage Capacity";
			
		case EUpgradeType::ChangeStructureResourceGeneration:
			return "Change Structure Resource Generation";

		case EUpgradeType::ChangeStructureResourceDrain:
			return "Change Structure Resource Drain";
			
		case EUpgradeType::UnlockPlayerEquipment:
			return "Unlock Player Equipment";
			
		default:
			return "Empty Upgrade Type";
		}
	}

	bool operator==(const FUpgrade& Other) const
	{
		return	Title == Other.Title &&
				Description == Other.Description &&
				UpgradeType == Other.UpgradeType;
	}
};