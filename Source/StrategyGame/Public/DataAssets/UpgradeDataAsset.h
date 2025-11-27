// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpgradeDataAsset.generated.h"

class AEquippableItem;
class AStructure;

UENUM(BlueprintType)
enum class EUpgradeType : uint8
{
	UnlockStructure = 0				UMETA(DisplayName="Unlock Structure"),
	UnlockPlayerEquipment = 1		UMETA(DisplayName="Unlock Player Equipment"),
	ChangeStructureProperties = 2	UMETA(DisplayName="Change Structure Properties"),
	ChangeEquipmentProperties = 3	UMETA(DisplayName="Change Equipment Properties")
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
	TSoftObjectPtr<UTexture2D> Icon;

	// How many research points it costs to unlock this upgrade.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data", meta=(ClampMin=1))
	int32 ResearchPointCost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Upgrade Data")
	EUpgradeType UpgradeType;

	// The upgrades that are required to be unlocked before this one.
	UPROPERTY(EditAnywhere, Category = "Upgrade Data")
	TSet<UUpgradeDataAsset*> PreRequisiteUpgrades;

public:

	UFUNCTION(BlueprintPure, Category="Upgrade Data")
	FString GetTitle() const;

	UFUNCTION(BlueprintPure, Category="Upgrade Data")
	FString GetDescription() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	bool IsUnlocked();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	bool ArePreRequisitesUnlocked();

	UFUNCTION(BlueprintPure, Category="Upgrade Data")
	EUpgradeType GetUpgradeType() const { return UpgradeType; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrade Data")
	TSet<UUpgradeDataAsset*>& GetPreRequisites() { return PreRequisiteUpgrades; }

	UFUNCTION(BlueprintPure, Category="Upgrade Data")
	int32 GetResearchPointCost() const;
};