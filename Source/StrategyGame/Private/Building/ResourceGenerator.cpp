// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ResourceGenerator.h"

#include "Components/ResourceGenerationComponent.h"


// Sets default values
AResourceGenerator::AResourceGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ResourceGenerationComponent = CreateDefaultSubobject<UResourceGenerationComponent>("Resource Generation");

	StructureCategory = EStructureCategory::ResourceGeneration;
}

// Called when the game starts or when spawned
void AResourceGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}