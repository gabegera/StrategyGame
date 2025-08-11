// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CustomActor.h"


// Sets default values
ACustomActor::ACustomActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	Super::BeginPlay();

	GetStrategyGameState()->ResourcesChangedDelegate.AddUniqueDynamic(this, &ThisClass::BP_OnResourcesChanged);
	GetStrategyGameState()->ResourcesChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);
}

void ACustomActor::OnResourcesChanged()
{
	
}

// Called every frame
void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AStrategyGameState* ACustomActor::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

