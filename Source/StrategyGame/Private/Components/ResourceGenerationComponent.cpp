// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourceGenerationComponent.h"

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

	ResourcesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UResourcesSubsystem>();
	GetWorld()->GetGameInstance()->GetSubsystem<UTimeSubsystem>()->OnTimePassed.AddUniqueDynamic(this, &ThisClass::OnTimePassed);
}

void UResourceGenerationComponent::OnTimePassed(float HoursPassed)
{
	ReactToOnTimePassed(HoursPassed);

	GenerateResources(HoursPassed);
}

void UResourceGenerationComponent::GenerateResources(float HoursPassed)
{
	if (!ConsumeResources(HoursPassed)) return;
	
	for (const TPair ResourceGenerated : ResourcesToGeneratePerHour)
	{
		ResourcesSubsystem->AddResources(ResourceGenerated.Key, ResourceGenerated.Value * HoursPassed);
	}
}

bool UResourceGenerationComponent::ConsumeResources(float HoursPassed)
{
	if (!HasEnoughResourcesToConsume(HoursPassed)) return false;
	
	for (const TPair ResourceConsumed : ResourcesToConsumePerHour)
	{
		ResourcesSubsystem->ConsumeResources(ResourceConsumed.Key, ResourceConsumed.Value * HoursPassed);
	}

	return true;
}

TArray<UResourceDataAsset*> UResourceGenerationComponent::GetGeneratedResources() const
{
	TArray<UResourceDataAsset*> OutArray;
	ResourcesToGeneratePerHour.GetKeys(OutArray);
	return OutArray;
}

bool UResourceGenerationComponent::HasEnoughResourcesToConsume(float HoursPassed) const
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

