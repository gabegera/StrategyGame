// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Structure.h"

#include "Pawns/RTSCamera.h"


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

bool AStructure::Select_Implementation(ARTSCamera* SelectInstigator)
{
	SelectInstigator->SetSelectedStructure(this);
	
	return Super::Select_Implementation(SelectInstigator);
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
	GetStrategyGameState()->StructureBuiltDelegate.Broadcast(this);
}

void AStructure::Recycle()
{
	RevertStorageCapacity();
	GetStrategyGameState()->StructureDestroyedDelegate.Broadcast(this);
	
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
		float ResourceAmount = Resource.Value * GetWorkerEfficiency();
		
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
		float ResourceAmount = Resource.Value * GetWorkerEfficiency();
		
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
		int32 ResourceStorageCapacity = GetStrategyGameState()->GetResourceCapacity(TargetResourceNode->GetResourceType());
		float ResourceStorageAmount = GetStrategyGameState()->GetResourceAmount(TargetResourceNode->GetResourceType());
		if (ResourceStorageCapacity >= ResourceStorageAmount + ResourcesToConsumePerSecond.FindRef(TargetResourceNode->GetResourceType()))
		{
			float AmountToDrain = ResourcesToConsumePerSecond.FindRef(TargetResourceNode->GetResourceType());
			AmountToDrain *= GetWorkerEfficiency();
			TargetResourceNode->DrainResource(AmountToDrain);
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

void AStructure::AssignWorkers(EWorkerType WorkerType, int32 Amount)
{
	if (Amount <= 0) return;
	
	if (IsWorkerCapacityFull())
	{
		GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Red, DisplayName + ": Worker Capacity is full.");
		return;
	}
	
	if (bRequiresScientists && WorkerType != EWorkerType::Scientist)
	{
		GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Red, DisplayName + " Requires Scientists");
		return;
	}

	if (GetStrategyGameState()->GetUnemployedPopulation(WorkerType) >= Amount)
	{
		AssignedWorkers.Add(WorkerType, GetWorkerCount(WorkerType) + Amount);
	}
	else
	{
		AssignedWorkers.Add(WorkerType, GetWorkerCount(WorkerType) + GetStrategyGameState()->GetUnemployedPopulation(WorkerType));
	}
}

void AStructure::AddMaxWorkers(EWorkerType WorkerType)
{
	if (GetAvailableWorkersSlots() < GetStrategyGameState()->GetPopulation(WorkerType))
	{
		AssignWorkers(WorkerType, GetStrategyGameState()->GetPopulation(WorkerType));
	}
	else
	{
		AssignWorkers(WorkerType, GetAvailableWorkersSlots());
	}
}

void AStructure::RemoveWorkers(EWorkerType WorkerType, int32 Amount)
{
	if (Amount <= 0) return;
	
	if (GetWorkerCount(WorkerType) >= Amount)
	{
		AssignedWorkers.Add(WorkerType, GetWorkerCount(WorkerType) - Amount);
	}
	else
	{
		AssignedWorkers.Add(WorkerType, 0);
	}
}

void AStructure::RemoveAllWorkers()
{
	RemoveWorkers(EWorkerType::Worker, GetWorkerCount(EWorkerType::Worker));
	RemoveWorkers(EWorkerType::Scientist, GetWorkerCount(EWorkerType::Scientist));
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

