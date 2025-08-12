// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Road.generated.h"

UCLASS()
class STRATEGYGAME_API ARoad : public ABuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARoad();

protected:

	UPROPERTY()
	TArray<ARoad*> ConnectedRoads;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual bool Recycle_Implementation(ARTSCamera* DestroyInstigator) override;

	virtual void UpdateBuildMaterials() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetConnectedRoads(TArray<ARoad*> NewConnectedRoads) { ConnectedRoads = NewConnectedRoads; }

	UFUNCTION(BlueprintCallable)
	void AddConnectedRoad(ARoad* RoadToAdd) { ConnectedRoads.Add(RoadToAdd); }
	
	void RemoveConnectedRoad(ARoad* RoadToRemove) { ConnectedRoads.Remove(RoadToRemove); ConnectedRoads.Shrink(); }
};
