// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UpgradesSubsystem.h"

#include "DataAssets/UpgradeDataAsset.h"
#include "Building/Structure.h"

bool UUpgradesSubsystem::UnlockUpgrade(UUpgradeDataAsset* InUpgrade)
{
	if (IsUpgradeUnlocked(InUpgrade)) return false;
	if (!ArePreRequisitesUnlocked(InUpgrade)) return false;
	
	UnlockedUpgrades.Add(InUpgrade);

	if (InUpgrade->GetUpgradeType() == EUpgradeType::UnlockStructure)
	{
		UnlockStructure(InUpgrade->GetTargetStructure().LoadSynchronous());
	}
	
	OnUpgradeUnlocked.Broadcast(InUpgrade);

	return true;
}

void UUpgradesSubsystem::UnlockStructure(const TSubclassOf<AStructure> InStructure)
{
	UnlockedStructures.Add(InStructure);
	OnStructureUnlocked.Broadcast(InStructure);
}

TSet<UUpgradeDataAsset*>& UUpgradesSubsystem::GetUnlockedUpgrades()
{
	return UnlockedUpgrades;
}

TSet<TSubclassOf<AStructure>>& UUpgradesSubsystem::GetUnlockedStructures()
{
	return UnlockedStructures;
}

TSet<TSubclassOf<AStructure>> UUpgradesSubsystem::GetUnlockedStructuresOfCategory(EStructureCategory StructureCategory)
{
	TSet<TSubclassOf<AStructure>> OutSet;

	for (TSubclassOf Structure : UnlockedStructures)
	{
		if (Structure.GetDefaultObject()->GetStructureCategory() == StructureCategory)
		{
			OutSet.Add(Structure);
		}
	}

	return OutSet;
}

bool UUpgradesSubsystem::IsUpgradeUnlocked(const UUpgradeDataAsset* InUpgrade)
{
	return UnlockedUpgrades.Contains(InUpgrade);
}

bool UUpgradesSubsystem::ArePreRequisitesUnlocked(UUpgradeDataAsset* InUpgrade)
{
	for (const UUpgradeDataAsset* RequiredUpgrade : InUpgrade->GetPreRequisites())
	{
		if (!IsUpgradeUnlocked(RequiredUpgrade))
		{
			return false;
		}
	}
	
	return true;
}
