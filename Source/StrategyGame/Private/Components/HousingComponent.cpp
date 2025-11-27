// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HousingComponent.h"

#include "Game/StrategyGameInstance.h"


// Sets default values for this component's properties
UHousingComponent::UHousingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = false;
}


// Called when the game starts
void UHousingComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UHousingComponent::OnStructureBuilt(AStructure* BuiltStructure)
{
	Super::OnStructureBuilt(BuiltStructure);

	if (BuiltStructure == OwningStructure)
	{
		RequestResidents();
	}
}

void UHousingComponent::RequestResidents()
{
	if (IsFullCapacity()) return;

	GetWorld()->GetGameInstance<UStrategyGameInstance>()->OnResidentRequested.Broadcast(OwningStructure, this);
}

void UHousingComponent::AssignResident(ACitizen* NewResident)
{
	if (Residents.Num() < HousingCapacity)
	{
		Residents.AddUnique(NewResident);
	}
}

TArray<ACitizen*>& UHousingComponent::GetResidents()
{
	return Residents;
}

int32 UHousingComponent::GetHousingCapacity() const
{
	return HousingCapacity;
}

bool UHousingComponent::IsFullCapacity() const
{
	return Residents.Num() >= HousingCapacity;
}

int32 UHousingComponent::GetNumAvailableResidents() const
{
	return HousingCapacity - Residents.Num();
}

