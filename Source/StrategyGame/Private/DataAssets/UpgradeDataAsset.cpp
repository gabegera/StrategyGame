// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/UpgradeDataAsset.h"

#include "Game/UpgradesSubsystem.h"


bool UUpgradeDataAsset::IsUnlocked()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUpgradesSubsystem>()->IsUpgradeUnlocked(this);
}

bool UUpgradeDataAsset::ArePreRequisitesUnlocked()
{
	return GetWorld()->GetGameInstance()->GetSubsystem<UUpgradesSubsystem>()->ArePreRequisitesUnlocked(this);
}
