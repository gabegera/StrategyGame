// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Structure.h"


// Sets default values
AStructure::AStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStructure::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AResourceNode::StaticClass()) &&
		bConsumesResourceFromNearbyNode)
	{
		AResourceNode* Resource = Cast<AResourceNode>(OtherActor);
		if (ResourcesToConsumePerSecond.Contains(Resource->GetResourceType()))
		{
			OverlappingResourceNodes.Add(Resource);
		}
	}
	
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AStructure::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AStructure::ActivateStructureEffects()
{
	if (bGeneratesResources) BeginGeneratingResources();
	if (bConsumesResources && !bConsumesResourceFromNearbyNode) BeginConsumingResources();
	if (bConsumesResources && bConsumesResourceFromNearbyNode) BeginDrainingResourceFromNode();
	if (bIncreasesStorageCapacity)
	{
		for (auto Resource : ResourcesToIncreaseStorage)
		{
			EResourceType ResourceType = Resource.Key;
			int32 Amount = Resource.Value;

			GetStrategyGameState()->IncreaseResourceStorage(ResourceType, Amount);
		}
	}
}

AResourceNode* AStructure::FindClosestResourceNode()
{
	if (OverlappingResourceNodes.IsEmpty()) return nullptr;
	
	float ClosestDistance = 0.0f;
	AActor* ClosestActor = nullptr;
	for (AActor* Node : OverlappingResourceNodes)
	{
		if (ClosestDistance <= 0.0f)
		{
			ClosestActor = Node;
			ClosestDistance = GetDistanceTo(Node);
		}
		else if (GetDistanceTo(Node) < ClosestDistance)
		{
			ClosestActor = Node;
		}
	}

	return Cast<AResourceNode>(ClosestActor);
}

void AStructure::BeginConstruction()
{
	Super::BeginConstruction();

	if (bConsumesResourceFromNearbyNode)
	{
		TargetResourceNode = FindClosestResourceNode();
		if (TargetResourceNode) TargetResourceNode->SetAssignedExtractor(this);
	}
}

void AStructure::CompleteConstruction()
{
	Super::CompleteConstruction();
	ActivateStructureEffects();
}

void AStructure::Recycle()
{
	RevertStorageCapacity();
	
	Super::Recycle();
}

void AStructure::RevertStorageCapacity()
{
	for (auto Resource : ResourcesToIncreaseStorage)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->DecreaseResourceStorage(ResourceType, ResourceAmount);
	}
}

void AStructure::BeginGeneratingResources()
{
	GetWorldTimerManager().SetTimer(ResourceGenerationTimer, this, &AStructure::GenerateResources, 1.0f, true);
}

void AStructure::GenerateResources()
{
	for (auto Resource : ResourcesToGeneratePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->AddResources(ResourceType, ResourceAmount);
	}
}

void AStructure::BeginConsumingResources()
{
	GetWorldTimerManager().SetTimer(ResourceConsumptionTimer, this, &AStructure::ConsumeResources, 1.0f, true);
}

void AStructure::ConsumeResources()
{
	for (auto Resource : ResourcesToConsumePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->ConsumeResources(ResourceType, ResourceAmount);
	}
}

void AStructure::BeginDrainingResourceFromNode()
{
	GetWorldTimerManager().SetTimer(ResourceDrainingTimer, this, &AStructure::DrainResourceFromNode, 1.0f, true);
}

void AStructure::DrainResourceFromNode()
{
	if (TargetResourceNode)
	{
		int32 ResourceCapacity = GetStrategyGameState()->GetResourceCapacity(TargetResourceNode->GetResourceType());
		int32 ResourceAmount = GetStrategyGameState()->GetResourceAmount(TargetResourceNode->GetResourceType());
		if (ResourceCapacity >= ResourceAmount + ResourcesToConsumePerSecond.FindRef(TargetResourceNode->GetResourceType()))
		{
			TargetResourceNode->DrainResource(ResourcesToConsumePerSecond.FindRef(TargetResourceNode->GetResourceType()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(950, 3.0f, FColor::Red, GetDisplayName() + " Can't extract resources, storage is full.");
		}
	}
	else
	{
		OverlappingResourceNodes.Remove(TargetResourceNode);

		if (IsOverlappingResourceNode())
		{
			TargetResourceNode = FindClosestResourceNode();
			if (TargetResourceNode) TargetResourceNode->SetAssignedExtractor(this);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(951, 3.0f, FColor::Red, GetDisplayName() + ": No Nearby Ore Nodes.");
		}
	}
}

void AStructure::UpdateBuildMaterials()
{
	Super::UpdateBuildMaterials();
}

// Called every frame
void AStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AStructure::IsBuildingPermitted()
{
	if (bConsumesResourceFromNearbyNode && !IsOverlappingResourceNode())
	{
		GEngine->AddOnScreenDebugMessage(800, 3.0f, FColor::Red, GetDisplayName() + " needs to be near the correct resource.");
		return false;
	}
	if (IsOverlappingResourceNode() && ConsumesResourcesFromNearbyNode() && FindClosestResourceNode()->GetAssignedExtractor())
	{
		GEngine->AddOnScreenDebugMessage(801, 3.0f, FColor::Red, "The Resource node already has an assigned extractor.");
		return false;
	}	
	
	return Super::IsBuildingPermitted();
}

