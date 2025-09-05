// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/BuildExclusionZone.h"


// Sets default values
ABuildExclusionZone::ABuildExclusionZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxTrigger = CreateDefaultSubobject<UBoxComponent>("Box Trigger");
	BoxTrigger->SetBoxExtent(FVector(2000.0f, 2000.0f, 2000.0f));
	BoxTrigger->SetCollisionProfileName("Trigger");
	BoxTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ABuildExclusionZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildExclusionZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

