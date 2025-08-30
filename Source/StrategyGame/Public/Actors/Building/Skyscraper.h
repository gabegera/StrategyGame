// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Skyscraper.generated.h"


// Alternate variant of AStructure that allows different modules to be stacked on top of each other to make a custom structure.
UCLASS()
class STRATEGYGAME_API ASkyscraper : public ABuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkyscraper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
