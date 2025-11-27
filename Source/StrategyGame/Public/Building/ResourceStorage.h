// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "ResourceStorage.generated.h"

// A Structure that includes a Resource Storage Component.
UCLASS(Abstract)
class STRATEGYGAME_API AResourceStorage : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceStorage();

protected:

	UPROPERTY(EditAnywhere, Category="Structure|Components")
	class UResourceStorageComponent* ResourceStorageComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
