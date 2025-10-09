// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UpgradesSubsystem.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeUnlocked, UUpgradeDataAsset*, UnlockedUpgrade);

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UUpgradesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	
	static inline TSet<TSoftObjectPtr<UUpgradeDataAsset>> UnlockedUpgrades;

public:

	static inline FOnUpgradeUnlocked OnUpgradeUnlocked;

	UFUNCTION(BlueprintCallable, Category="Upgrades Subsystem")
	static void UnlockUpgrade(UUpgradeDataAsset* Upgrade);

	UFUNCTION(BlueprintCallable, Category="Upgrades Subsystem")
	static TSet<TSoftObjectPtr<UUpgradeDataAsset>>& GetUnlockedUpgrades();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrades Subsystem")
	static bool IsUpgradeUnlocked(TSoftObjectPtr<UUpgradeDataAsset> InUpgrade);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrdes Subsystem")
	static bool ArePreRequisitesUnlocked(TSoftObjectPtr<UUpgradeDataAsset> InUpgrade);
};
