// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/House.h"

#include "Components/HousingComponent.h"


// Sets default values
AHouse::AHouse()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HousingComponent = CreateDefaultSubobject<UHousingComponent>("Housing");

	StructureCategory = EStructureCategory::Housing;
}

// Called when the game starts or when spawned
void AHouse::BeginPlay()
{
	Super::BeginPlay();
	
}

