// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Skyscraper.h"


// Sets default values
ASkyscraper::ASkyscraper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkyscraper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkyscraper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

