// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/UpgradeDataAsset.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UpgradesSubsystem.generated.h"

enum class EStructureCategory : uint8;
class AStructure;
class UUpgradeDataAsset;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpgradeUnlockedSignature, UUpgradeDataAsset*, UnlockedUpgrade);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructureUnlockedSignature, TSubclassOf<AStructure>, UnlockedStructure);

/**
 * The system that manages unlocking upgrades and storing them for future use.
 */
UCLASS(DisplayName="Upgrades Subsystem")
class STRATEGYGAME_API UUpgradesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, Category="Upgrades Subsystem")
	TSet<UUpgradeDataAsset*> UnlockedUpgrades;

	UPROPERTY(VisibleAnywhere, Category="Upgrades Subsystem")
	TSet<TSubclassOf<AStructure>> UnlockedStructures;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FUpgradeUnlockedSignature OnUpgradeUnlocked;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FStructureUnlockedSignature OnStructureUnlocked;

	/**
	 * Attempts to unlock an input upgrade based on it's pre-requisites.
	 * @param InUpgrade The Upgrade to unlock.
	 * @return Returns if the upgrade was successful.
	 */
	UFUNCTION(BlueprintCallable, Category="Upgrades Subsystem")
	bool UnlockUpgrade(UUpgradeDataAsset* InUpgrade);

	UFUNCTION(BlueprintCallable, Category="Upgradse Subsystem")
	void UnlockStructure(const TSubclassOf<AStructure> InStructure);

	UFUNCTION(BlueprintPure, Category="Upgrades Subsystem")
	TSet<UUpgradeDataAsset*>& GetUnlockedUpgrades();

	UFUNCTION(BlueprintPure, Category="Upgrades Subsystem")
	TSet<TSubclassOf<AStructure>>& GetUnlockedStructures();

	UFUNCTION(BlueprintPure)
	TSet<TSubclassOf<AStructure>> GetUnlockedStructuresOfCategory(EStructureCategory StructureCategory);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrades Subsystem")
	bool IsUpgradeUnlocked(const UUpgradeDataAsset* InUpgrade);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Upgrdes Subsystem")
	bool ArePreRequisitesUnlocked(UUpgradeDataAsset* InUpgrade);
};
