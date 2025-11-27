// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ResourceHarvester.h"

#include "Components/ResourceHarvestingComponent.h"
#include "Components/WorkersComponent.h"


// Sets default values
AResourceHarvester::AResourceHarvester()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StructureCategory = EStructureCategory::ResourceGeneration;

	ResourceHarvestingComponent = CreateDefaultSubobject<UResourceHarvestingComponent>("Resource Harvesting");

	WorkersComponent = CreateDefaultSubobject<UWorkersComponent>("Workers");
}

// Called when the game starts or when spawned
void AResourceHarvester::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AResourceHarvester::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

