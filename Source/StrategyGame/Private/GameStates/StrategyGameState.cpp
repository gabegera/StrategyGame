// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/StrategyGameState.h"


AStrategyGameState::AStrategyGameState()
{
	ResourceInventory.Add(EResourceType::Metal, 60);
	ResourceInventory.Add(EResourceType::AlienMaterial, 0);
	ResourceInventory.Add(EResourceType::Food, 20);
	ResourceInventory.Add(EResourceType::Power, 200);
	ResourceInventory.Add(EResourceType::Workers, 20);
	ResourceInventory.Add(EResourceType::Scientists, 5);

	MaximumResources.Add(EResourceType::Metal, 100);
	MaximumResources.Add(EResourceType::AlienMaterial, 20);
	MaximumResources.Add(EResourceType::Food, 100);
	MaximumResources.Add(EResourceType::Power, 200);
	MaximumResources.Add(EResourceType::Workers, 20);
	MaximumResources.Add(EResourceType::Scientists, 5);
}

void AStrategyGameState::BeginPlay()
{
	Super::BeginPlay();

	for (auto Resource : ResourceInventory)
	{
		EResourceType ResourceType = Resource.Key;
		int32 Amount = Resource.Value;
		
		if (Amount > GetResourceCapacity(ResourceType))
		{
			ResourceInventory.Add(ResourceType, GetResourceCapacity(ResourceType));
		}
	}
}

bool AStrategyGameState::AddResources(EResourceType ResourceType, int32 Amount)
{
	if (GetResourceAmount(ResourceType) == GetResourceCapacity(ResourceType))
	{
		switch (ResourceType)
		{
		case EResourceType::Metal:
			GEngine->AddOnScreenDebugMessage(900, 3.0f, FColor::Red, "METAL storage is full.");
			break;
		case EResourceType::AlienMaterial:
			GEngine->AddOnScreenDebugMessage(901, 3.0f, FColor::Red, "ALIEN MATERIAL storage is full.");
			break;
		case EResourceType::Food:
			GEngine->AddOnScreenDebugMessage(902, 3.0f, FColor::Red, "FOOD storage is full.");
			break;
		case EResourceType::Power:
			GEngine->AddOnScreenDebugMessage(903, 3.0f, FColor::Red, "POWER capacity is full.");
			break;
		case EResourceType::Workers:
			GEngine->AddOnScreenDebugMessage(904, 3.0f, FColor::Red, "WORKER capacity is full.");
			break;
		case EResourceType::Scientists:
			GEngine->AddOnScreenDebugMessage(905, 3.0f, FColor::Red, "SCIENTIST capacity is full.");
			break;
		}
		return false;
	}
	
	ResourceInventory.Add(ResourceType, GetResourceAmount(ResourceType) + Amount);

	if (GetResourceAmount(ResourceType) > GetResourceCapacity(ResourceType))
	{
		ResourceInventory.Add(ResourceType, GetResourceCapacity(ResourceType));
	}

	return true;
}

bool AStrategyGameState::ConsumeResources(EResourceType ResourceType, int32 Amount)
{
	if (GetResourceAmount(ResourceType) - Amount < 0)
	{
		switch (ResourceType)
		{
		case EResourceType::Metal:
			GEngine->AddOnScreenDebugMessage(906, 3.0f, FColor::Red, "Attempted to remove more METAL than was available.");
			break;
		case EResourceType::AlienMaterial:
			GEngine->AddOnScreenDebugMessage(907, 3.0f, FColor::Red, "Attempted to remove more ALIEN MATERIAL than was available.");
			break;
		case EResourceType::Food:
			GEngine->AddOnScreenDebugMessage(908, 3.0f, FColor::Red, "Attempted to remove more FOOD than was available.");
			break;
		case EResourceType::Power:
			GEngine->AddOnScreenDebugMessage(909, 3.0f, FColor::Red, "Attempted to remove more POWER than was available.");
			break;
		case EResourceType::Workers:
			GEngine->AddOnScreenDebugMessage(910, 3.0f, FColor::Red, "Attempted to remove more WORKERS than was available.");
			break;
		case EResourceType::Scientists:
			GEngine->AddOnScreenDebugMessage(911, 3.0f, FColor::Red, "Attempted to remove more SCIENTISTS than was available.");
			break;
		}
		return false;
	}
	
	ResourceInventory.Add(ResourceType, GetResourceAmount(ResourceType) - Amount);

	return true;
}

void AStrategyGameState::IncreaseResourceStorage(EResourceType ResourceType, int32 IncreaseAmount)
{
	MaximumResources.Add(ResourceType, GetResourceCapacity(ResourceType) + IncreaseAmount);
}

void AStrategyGameState::DecreaseResourceStorage(EResourceType ResourceType, int32 DecreaseAmount)
{
	MaximumResources.Add(ResourceType, FMath::Clamp(GetResourceCapacity(ResourceType) - DecreaseAmount, 0, GetResourceCapacity(ResourceType)));
}
