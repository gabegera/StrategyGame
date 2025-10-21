// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SquadManager.h"

void USquadManager::SpawnSquadMembers()
{
	
}

void USquadManager::SetAttackTarget(AActor* Target)
{
	if (SquadMembers.IsEmpty()) return;
	
	for (auto SquadMember : SquadMembers)
	{
		// code here...
	}
}

void USquadManager::SetTargetPosition(FVector InPosition)
{
	if (SquadMembers.IsEmpty()) return;
	
	for (auto SquadMember : SquadMembers)
	{
		// code here...
	}
}
