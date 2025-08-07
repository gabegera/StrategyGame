// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "StrategyGameState.generated.h"


UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Metal				UMETA(DisplayName="Metal"),
	AlienMaterial		UMETA(DisplayName="Alien Material"),
	Food				UMETA(DisplayName="Food"),
	Power				UMETA(DisplayName="Power"),
	Workers				UMETA(DisplayName="Workers"),
	Scientists			UMETA(DisplayName="Scientists"),
};

UCLASS()
class STRATEGYGAME_API AStrategyGameState : public AGameStateBase
{
	GENERATED_BODY()

	AStrategyGameState();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TMap<EResourceType, int32> ResourceInventory;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TMap<EResourceType, int32> MaximumResources;

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	int32 GetResourceAmount(EResourceType ResourceType) { return ResourceInventory.FindRef(ResourceType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Resources")
	int32 GetMaximumResource(EResourceType ResourceType) { return MaximumResources.FindRef(ResourceType); }

	// Attempts to add resources to the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	bool AddResources(EResourceType ResourceType, int32 Amount);

	// Attempts to remove resources from the ResourceInventory Map. Returns if it was successful.
	UFUNCTION(BlueprintCallable, Category="Resources")
	bool ConsumeResources(EResourceType ResourceType, int32 Amount);

	UFUNCTION(BlueprintCallable, Category="Resources")
	void IncreaseResourceStorage(EResourceType ResourceType, int32 IncreaseAmount);
	
	UFUNCTION(BlueprintCallable, Category="Resources")
	void DecreaseResourceStorage(EResourceType ResourceType, int32 DecreaseAmount);
};
