// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UnlocksSubsystem.h"

#include "StrategyStatics.h"
#include "DataAssets/UpgradeDataAsset.h"
#include "Game/ResourcesSubsystem.h"

TSet<UUpgradeDataAsset*> UUnlocksSubsystem::SetUnlockedUpgrades(const TSet<UUpgradeDataAsset*> InUpgrades)
{
	return UnlockedUpgrades = InUpgrades;
}

bool UUnlocksSubsystem::UnlockUpgrade(UUpgradeDataAsset* InUpgrade)
{
	if (IsUpgradeUnlocked(InUpgrade)) return false;
	if (!ArePreRequisitesUnlocked(InUpgrade)) return false;
	if (!HasEnoughResearchPoints(InUpgrade)) return false;
	
	UnlockedUpgrades.Add(InUpgrade);

	UResourceDataAsset* ResearchPointDataAsset = UStrategyStatics::GetResourceByName("Research Point");
	GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->ConsumeResources(ResearchPointDataAsset, InUpgrade->GetResearchPointCost());
	
	OnUpgradeUnlocked.Broadcast(InUpgrade);

	return true;
}

TSet<UUpgradeDataAsset*>& UUnlocksSubsystem::GetUnlockedUpgrades()
{
	return UnlockedUpgrades;
}

TSet<UUpgradeDataAsset*> UUnlocksSubsystem::GetUnlockedUpgradeOfType(const EUpgradeType UpgradeType)
{
	TSet<UUpgradeDataAsset*> OutSet;

	for (UUpgradeDataAsset* Upgrade : UnlockedUpgrades)
	{
		if (Upgrade->GetUpgradeType() == UpgradeType)
		{
			OutSet.Add(Upgrade);
		}
	}

	return OutSet;
}

bool UUnlocksSubsystem::IsUpgradeUnlocked(UUpgradeDataAsset* InUpgrade) const
{
	return UnlockedUpgrades.Contains(InUpgrade);
}

bool UUnlocksSubsystem::HasEnoughResearchPoints(UUpgradeDataAsset* InUpgrade) const
{
	if (UResourceDataAsset* ResearchPointsResource = UStrategyStatics::GetResourceByName("Research Point"))
	{
		const int32 NumOfResearchPoints = GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->GetResourceAmountInt(ResearchPointsResource);
		return NumOfResearchPoints >= InUpgrade->GetResearchPointCost();
	}

	return false;
}

bool UUnlocksSubsystem::ArePreRequisitesUnlocked(UUpgradeDataAsset* InUpgrade) const
{
	for (UUpgradeDataAsset* RequiredUpgrade : InUpgrade->GetPreRequisites())
	{
		if (!IsUpgradeUnlocked(RequiredUpgrade))
		{
			return false;
		}
	}
	
	return true;
}
