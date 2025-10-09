// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UpgradesSubsystem.h"

#include "UpgradeDataAsset.h"

void UUpgradesSubsystem::UnlockUpgrade(UUpgradeDataAsset* Upgrade)
{
	UnlockedUpgrades.Add(Upgrade);
	
	OnUpgradeUnlocked.Broadcast(Upgrade);
}

TSet<TSoftObjectPtr<UUpgradeDataAsset>>& UUpgradesSubsystem::GetUnlockedUpgrades()
{
	return UnlockedUpgrades;
}

bool UUpgradesSubsystem::IsUpgradeUnlocked(const TSoftObjectPtr<UUpgradeDataAsset> InUpgrade)
{
	return UnlockedUpgrades.Contains(InUpgrade);
}

bool UUpgradesSubsystem::ArePreRequisitesUnlocked(const TSoftObjectPtr<UUpgradeDataAsset> InUpgrade)
{
	for (auto RequiredUpgrade : InUpgrade.LoadSynchronous()->GetPreRequisites())
	{
		if (!IsUpgradeUnlocked(RequiredUpgrade))
		{
			return false;
		}
	}
	
	return true;
}
