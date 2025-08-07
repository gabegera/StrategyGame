// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildableStructure.h"
#include "StorageBuilding.generated.h"

UCLASS()
class STRATEGYGAME_API AStorageBuilding : public ABuildableStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStorageBuilding();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
