// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/StructureExample.h"

#include "Components/WorkersComponent.h"


// Sets default values
AStructureExample::AStructureExample()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StructureName = FText::FromString("Example");

	HousingComponent = CreateDefaultSubobject<UHousingComponent>("Housing");
	ResourceGenerationComponent = CreateDefaultSubobject<UResourceGenerationComponent>("Resource Generation");
	ResourceHarvestingComponent = CreateDefaultSubobject<UResourceHarvestingComponent>("Resource Harvesting");
	ResourceStorageComponent = CreateDefaultSubobject<UResourceStorageComponent>("Resource Storage");
	WorkersComponent = CreateDefaultSubobject<UWorkersComponent>("Workers");
}

// Called when the game starts or when spawned
void AStructureExample::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AStructureExample::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

