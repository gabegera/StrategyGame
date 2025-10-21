// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HousingComponent.h"


// Sets default values for this component's properties
UHousingComponent::UHousingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UHousingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UHousingComponent::AssignResident(ACitizen* NewResident)
{
	if (Residents.Num() < HousingCapacity)
	{
		Residents.Add(NewResident);
	}
}

TArray<ACitizen*>& UHousingComponent::GetResidents()
{
	return Residents;
}

bool UHousingComponent::IsFullCapacity()
{
	return Residents.Num() >= HousingCapacity;
}

int32 UHousingComponent::GetNumAvailableResidents()
{
	return HousingCapacity - Residents.Num();
}

