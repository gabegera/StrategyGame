// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Upgrade.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UpgradesSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeUnlocked, FUpgrade, UnlockedUpgrade);

/**
 * 
 */
UCLASS()
class STRATEGYGAME_API UUpgradesSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	static inline FOnUpgradeUnlocked OnUpgradeUnlocked;

	UFUNCTION(BlueprintCallable, Category="Upgrades Subsystem|Delegates")
	static void BroadcastUpgradeUnlocked(const FUpgrade UnlockedUpgrade) { OnUpgradeUnlocked.Broadcast(UnlockedUpgrade); }
};
