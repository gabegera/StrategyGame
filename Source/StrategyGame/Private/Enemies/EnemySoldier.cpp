// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemySoldier.h"


// Sets default values
AEnemySoldier::AEnemySoldier()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySoldier::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemySoldier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemySoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

