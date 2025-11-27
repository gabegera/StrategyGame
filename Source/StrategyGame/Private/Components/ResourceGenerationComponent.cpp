// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourceGenerationComponent.h"

#include "Building/Structure.h"
#include "Components/WorkersComponent.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/TimeSubsystem.h"


// Sets default values for this component's properties
UResourceGenerationComponent::UResourceGenerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UResourceGenerationComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningStructure)
	{
		OwningStructureWorkersComponent = OwningStructure->GetComponentByClass<UWorkersComponent>();
	}

	ResourcesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UResourcesSubsystem>();
	GetWorld()->GetGameInstance()->GetSubsystem<UTimeSubsystem>()->OnTimePassed.AddUniqueDynamic(this, &ThisClass::OnTimePassed);
}

void UResourceGenerationComponent::OnTimePassed(const float HoursPassed)
{
	if (!IsActive()) return;

	if (ConsumeResources(HoursPassed))
	{
		GenerateResources(HoursPassed);
	}
}

void UResourceGenerationComponent::GenerateResources(const float HoursPassed)
{
	for (const TPair ResourceGenerated : ResourcesToGeneratePerHour)
	{
		const float AmountToGenerate = ResourceGenerated.Value * OwningStructureWorkersComponent->GetWorkersPercentage() * HoursPassed;
		ResourcesSubsystem->AddResources(ResourceGenerated.Key, AmountToGenerate);
	}
}

bool UResourceGenerationComponent::ConsumeResources(const float HoursPassed)
{
	if (!HasEnoughResourcesToConsume(HoursPassed)) return false;
	
	for (const TPair ResourceConsumed : ResourcesToConsumePerHour)
	{
		const float AmountToConsume = ResourceConsumed.Value * OwningStructureWorkersComponent->GetWorkersPercentage() * HoursPassed;
		ResourcesSubsystem->ConsumeResources(ResourceConsumed.Key, AmountToConsume);
	}

	return true;
}

TArray<UResourceDataAsset*> UResourceGenerationComponent::GetGeneratedResources() const
{
	TArray<UResourceDataAsset*> OutArray;
	ResourcesToGeneratePerHour.GetKeys(OutArray);
	return OutArray;
}

float UResourceGenerationComponent::GetGeneratedResourceAmount(const UResourceDataAsset* InResource) const
{
	return ResourcesToGeneratePerHour.FindRef(InResource);
}

bool UResourceGenerationComponent::GetCostsResources() const
{
	return bCostsResources;
}

bool UResourceGenerationComponent::HasEnoughResourcesToConsume(const float HoursPassed) const
{
	for (const TPair ResourceToConsume : ResourcesToConsumePerHour)
	{
		if (ResourcesSubsystem->GetResourceAmount(ResourceToConsume.Key) < ResourceToConsume.Value * HoursPassed)
		{
			return false;
		}
	}

	return true;
}

