// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/UpgradeDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UnlocksSubsystem.generated.h"

class UUpgradeDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpgradeUnlockedSignature, UUpgradeDataAsset*, UnlockedUpgrade);

/**
 * The system that manages unlocking Upgrades and storing them for future use.
 */
UCLASS(DisplayName="Unlocks Subsystem")
class STRATEGYGAME_API UUnlocksSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category="Unlocks Subsystem")
	TSet<UUpgradeDataAsset*> UnlockedUpgrades;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FUpgradeUnlockedSignature OnUpgradeUnlocked;

	UFUNCTION(BlueprintCallable, Category="Unlocks Subsystem")
	TSet<UUpgradeDataAsset*> SetUnlockedUpgrades(TSet<UUpgradeDataAsset*> InUnlocks);

	/**
	 * Attempts to unlock an input upgrade based on it's pre-requisites.
	 * @param InUpgrade The Upgrade to unlock.
	 * @return Returns if the upgrade was successful.
	 */
	UFUNCTION(BlueprintCallable, Category="Unlocks Subsystem")
	bool UnlockUpgrade(UUpgradeDataAsset* InUpgrade);

	UFUNCTION(BlueprintPure, Category="Unlocks Subsystem")
	TSet<UUpgradeDataAsset*>& GetUnlockedUpgrades();

	UFUNCTION(BlueprintPure, Category="Unlocks Subsystem")
	TSet<UUpgradeDataAsset*> GetUnlockedUpgradeOfType(EUpgradeType UpgradeType);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Unlocks Subsystem")
	bool IsUpgradeUnlocked(UUpgradeDataAsset* InUpgrade) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Unlocks Subsystem")
	bool HasEnoughResearchPoints(UUpgradeDataAsset* InUpgrade) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrdes Subsystem")
	bool ArePreRequisitesUnlocked(UUpgradeDataAsset* InUpgrade) const;
};
