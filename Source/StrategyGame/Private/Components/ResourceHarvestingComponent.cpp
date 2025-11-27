// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourceHarvestingComponent.h"

#include "Building/Structure.h"
#include "Components/SphereComponent.h"
#include "Components/WorkersComponent.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/TimeSubsystem.h"
#include "Interfaces/ResourcesInterface.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UResourceHarvestingComponent::UResourceHarvestingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("HarvestingRange");
	SphereComponent->SetSphereRadius(1200);
}


// Called when the game starts
void UResourceHarvestingComponent::BeginPlay()
{
	Super::BeginPlay();

	if (OwningStructure)
	{
		OwningStructureWorkersComponent = OwningStructure->GetComponentByClass<UWorkersComponent>();
	}

	GetWorld()->GetGameInstance()->GetSubsystem<UTimeSubsystem>()->OnTimePassed.AddUniqueDynamic(this, &ThisClass::OnTimePassed);
}

void UResourceHarvestingComponent::PostInitProperties()
{
	Super::PostInitProperties();

	if (GetOwner() && SphereComponent->GetAttachParent() != GetOwner()->GetRootComponent())
	{
		SphereComponent->SetupAttachment(GetOwner()->GetRootComponent());
	}
}

void UResourceHarvestingComponent::OnTimePassed(const float HoursPassed)
{
	HarvestNearbyResources(HoursPassed);
}

void UResourceHarvestingComponent::HarvestNearbyResources(const float HoursPassed)
{
	const FVector TraceStart = SphereComponent->GetComponentLocation();
	const FVector TraceEnd = TraceStart;
	const float Radius = SphereComponent->GetScaledSphereRadius();
	const ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Visibility);
	constexpr bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> Hits;
	constexpr bool bIgnoreSelf = true;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, Radius, TraceTypeQuery, bTraceComplex, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hits, bIgnoreSelf);

	if (Hits.IsEmpty())
	{
		FString PrintString = GetOwner()->GetName();
		PrintString.Append(" has no nearby resources to harvest.");
		GEngine->AddOnScreenDebugMessage(800, 1.0f, FColor::Red, PrintString);
	}

	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor() && Hit.GetActor()->Implements<UResourcesInterface>())
		{
			IResourcesInterface* ResourcesInterface = Cast<IResourcesInterface>(Hit.GetActor());
			UResourceDataAsset* ResourceType = ResourcesInterface->TryGetResourceType();

			if (ResourcesToHarvestPerHour.Contains(ResourceType))
			{
				float DrainAmount;
				if (OwningStructureWorkersComponent)
				{
					DrainAmount = ResourcesToHarvestPerHour.FindRef(ResourceType) * OwningStructureWorkersComponent->GetWorkersPercentage() * HoursPassed;
				}
				else
				{
					DrainAmount = ResourcesToHarvestPerHour.FindRef(ResourceType) * HoursPassed;
				}

				GetWorld()->GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->AddResources(ResourceType, ResourcesInterface->TryDrainResource(DrainAmount));

				// Returns so that only one resource gets drained at a time.
				return;
			}
		}
	}
}

TArray<UResourceDataAsset*> UResourceHarvestingComponent::GetHarvestingResources() const
{
	TArray<UResourceDataAsset*> OutArray;
	ResourcesToHarvestPerHour.GetKeys(OutArray);
	return OutArray;
}

