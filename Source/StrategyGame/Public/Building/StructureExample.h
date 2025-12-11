// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "Components/HousingComponent.h"
#include "Components/ResourceGenerationComponent.h"
#include "Components/ResourceHarvestingComponent.h"
#include "Components/ResourceStorageComponent.h"
#include "StructureExample.generated.h"

UCLASS()
class STRATEGYGAME_API AStructureExample : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStructureExample();

protected:

	UPROPERTY(EditAnywhere)
	UHousingComponent* HousingComponent;

	UPROPERTY(EditAnywhere)
	UResourceGenerationComponent* ResourceGenerationComponent;

	UPROPERTY(EditAnywhere)
	UResourceHarvestingComponent* ResourceHarvestingComponent;

	UPROPERTY(EditAnywhere)
	UResourceStorageComponent* ResourceStorageComponent;

	UPROPERTY(EditAnywhere)
	UWorkersComponent* WorkersComponent;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
