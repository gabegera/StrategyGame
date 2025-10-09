// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeDataAsset.h"

#include "Game/UpgradesSubsystem.h"


bool UUpgradeDataAsset::IsUnlocked()
{
	return UUpgradesSubsystem::IsUpgradeUnlocked(this);
}

bool UUpgradeDataAsset::ArePreRequisitesUnlocked()
{
	return UUpgradesSubsystem::ArePreRequisitesUnlocked(this);
}
