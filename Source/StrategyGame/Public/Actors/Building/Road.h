// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Components/SplineMeshComponent.h"
#include "Road.generated.h"

UCLASS()
class STRATEGYGAME_API ARoad : public ABuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoad();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineMeshComponent* SplineMesh = nullptr;

	UPROPERTY() FVector RoadStartPos = FVector::ZeroVector;
	UPROPERTY() FVector RoadEndPos = FVector::ZeroVector;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void MoveBuilding(FVector NewLocation) override;

	virtual void PlaceBuilding() override;

	virtual void UpdateBuildMaterials() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
