// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "ResourceHarvester.generated.h"

UCLASS(Abstract)
class STRATEGYGAME_API AResourceHarvester : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceHarvester();

protected:

	UPROPERTY(EditAnywhere, Category="Structure|Components")
	class UResourceHarvestingComponent* ResourceHarvestingComponent;

	UPROPERTY(EditAnywhere, Category="Structure|Components")
	class UWorkersComponent* WorkersComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
