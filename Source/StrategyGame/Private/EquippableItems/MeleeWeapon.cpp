// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableItems/MeleeWeapon.h"


// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMeleeWeapon::UseItemPrimary()
{
	Super::UseItemPrimary();

	LightAttack();
}

void AMeleeWeapon::UseItemSecondary()
{
	Super::UseItemSecondary();

	HeavyAttack();
}

void AMeleeWeapon::LightAttack()
{
	
}

void AMeleeWeapon::HeavyAttack()
{
	
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

