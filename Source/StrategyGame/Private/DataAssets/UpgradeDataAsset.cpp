// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/UpgradeDataAsset.h"

#include "Game/UnlocksSubsystem.h"


FString UUpgradeDataAsset::GetTitle() const
{
	return Title;
}

FString UUpgradeDataAsset::GetDescription() const
{
	return Description;
}

bool UUpgradeDataAsset::IsUnlocked()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUnlocksSubsystem>()->IsUpgradeUnlocked(this);
}

bool UUpgradeDataAsset::ArePreRequisitesUnlocked()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUnlocksSubsystem>()->ArePreRequisitesUnlocked(this);
}

int32 UUpgradeDataAsset::GetResearchPointCost() const
{
	return ResearchPointCost;
}
