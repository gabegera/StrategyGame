// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "ResourceGenerator.generated.h"

// A Structure that includes a Resource Generation Component.
UCLASS()
class STRATEGYGAME_API AResourceGenerator : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceGenerator();

protected:

	UPROPERTY(EditAnywhere, Category="Structure|Components")
	class UResourceGenerationComponent* ResourceGenerationComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
