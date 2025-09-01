// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyscraperModule.h"
#include "Structure.h"
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
	
	UPROPERTY(EditAnywhere, Category="Skyscraper|Modules")
	TSubclassOf<ASkyscraperModule> ModuleClass;

	UPROPERTY(EditAnywhere, Category="Skyscraper|Modules", meta=(ClampMin=1, ClampMax=5))
	int32 MaxSkyscraperSections = 3;
	
	UPROPERTY(VisibleAnywhere, Category="Skyscraper|Modules")
	TArray<ASkyscraperModule*> Modules;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CompleteConstruction() override;

	UFUNCTION(BlueprintCallable, Category="Skyscraper")
	void BuildModules();

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	UFUNCTION(BlueprintCallable, Category="Skyscraper")
	void AddModule(TSubclassOf<ASkyscraperModule> ModuleToAdd);

	virtual void Recycle() override;

	virtual bool Select_Implementation(ARTSCamera* SelectInstigator) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	
};

