// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Structure.h"
#include "House.generated.h"

// A Structure that includes a Housing Component.
UCLASS(Abstract)
class STRATEGYGAME_API AHouse : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHouse();

protected:

	UPROPERTY(EditAnywhere, Category="Structure|Components")
	class UHousingComponent* HousingComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
