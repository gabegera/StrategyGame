// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/ResourceStorage.h"

#include "Components/ResourceStorageComponent.h"


// Sets default values
AResourceStorage::AResourceStorage()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ResourceStorageComponent = CreateDefaultSubobject<UResourceStorageComponent>("Resource Storage");

	StructureCategory = EStructureCategory::Storage;
}

// Called when the game starts or when spawned
void AResourceStorage::BeginPlay()
{
	Super::BeginPlay();
	
}