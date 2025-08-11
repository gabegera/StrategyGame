// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStates/StrategyGameState.h"
#include "CustomActor.generated.h"

UCLASS()
class STRATEGYGAME_API ACustomActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACustomActor();

protected:

	UPROPERTY() FResourcesChangedDelegate ResourcesChangedDelegate;

	UPROPERTY() AStrategyGameState* StrategyGameState = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnResourcesChanged")
	void BP_OnResourcesChanged();

	UFUNCTION()
	virtual void OnResourcesChanged();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintGetter)
	AStrategyGameState* GetStrategyGameState();
};
