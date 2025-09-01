// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Building/Structure.h"
#include "DataTables/StructureData.h"
#include "Interfaces/BuildingInterface.h"
#include "SkyscraperModule.generated.h"

// This class is only meant to be used as a child actor for ASkyscraper.
UCLASS()
class STRATEGYGAME_API ASkyscraperModule : public AStructure
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASkyscraperModule();

protected:

	// This mesh will be used if the module is at the top of the skyscraper.
	UPROPERTY(EditAnywhere, Category="Mesh")
	TSoftObjectPtr<UStaticMesh> TopMesh = nullptr;

	// This mesh will be used if the module is not at the top of the skyscraper.
	UPROPERTY(EditAnywhere, Category="Mesh")
	TSoftObjectPtr<UStaticMesh> DefaultMesh = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	virtual bool Select_Implementation(ARTSCamera* SelectInstigator) override;
	virutal bool Recycle_Implementation(ARTSCamera* DestroyInstigator) override;

	UFUNCTION(BlueprintCallable)
	void SwitchToTopMesh();

	UFUNCTION(BlueprintCallable)
	void SwitchToDefaultMesh();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
