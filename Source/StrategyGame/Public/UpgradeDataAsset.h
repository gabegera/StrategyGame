// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpgradeDataAsset.generated.h"

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	UnlockStructure = 0						UMETA(DisplayName="Unlock Structure"),
	UnlockPlayerEquipment = 1				UMETA(DisplayName="Unlock Player Equipment"),
};

/**
 * An Upgrade that can unlock new structures / items or can improve existing ones.
 */
UCLASS(BlueprintType)
class STRATEGYGAME_API UUpgradeDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data")
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data")
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data")
	EUpgradeType UpgradeType;

	// Either a new structure being unlocked or the structure that is getting an upgrade.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data", meta=(EditCondition="UpgradeType != EUpgradeType::UnlockPlayerEquipment", EditConditionHides))
	TSoftClassPtr<class ABuildable> TargetStructure;

	// Either a new equippable being unlocked or the equippable that is getting an upgrade.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data", meta=(EditCondition="UpgradeType == EUpgradeType::UnlockPlayerEquipment", EditConditionHides))
	TSoftClassPtr<class AEquippableItem> TargetEquippable;

	// The upgrades that are required to be unlocked before this one.
	UPROPERTY(EditAnywhere, Category = "Upgrade Data")
	TSet<UUpgradeDataAsset*> PreRequisiteUpgrades;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	bool IsUnlocked();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	bool ArePreRequisitesUnlocked();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	TSet<UUpgradeDataAsset*>& GetPreRequisites() { return PreRequisiteUpgrades; }
};