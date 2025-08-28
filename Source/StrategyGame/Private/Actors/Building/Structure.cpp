// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Structure.h"

#include "Actors/Building/PowerLine.h"
#include "GameFramework/GameSession.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pawns/RTSCamera.h"


// Sets default values
AStructure::AStructure()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh->SetCollisionProfileName("Selectable");

	StructureText = CreateDefaultSubobject<UTextRenderComponent>("Structure Text");
	StructureText->SetupAttachment(StaticMesh);
	StructureText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	StructureText->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	StructureText->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	StructureText->SetWorldSize(256.0f);
}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();
}

void AStructure::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	StructureText->SetText(FText::FromString(DisplayName));
	StructureText->SetRelativeLocation(FVector(0.0f, 0.0f, BuildingBounds->GetScaledBoxExtent().Z * 2 + 50.0f));
}

void AStructure::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AStructure::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	OverlappingResourceNodes.Remove(OtherActor);
}

bool AStructure::Select_Implementation(ARTSCamera* SelectInstigator)
{
	SelectInstigator->SetSelectedStructure(this);
	
	return true;
}

bool AStructure::ConnectPower_Implementation(APowerLine* PowerLine)
{
	ConnectedPowerLines.AddUnique(PowerLine);
	PowerLine->SetPowerFeeder(this);
	
	return true;
}

bool AStructure::DisconnectPower_Implementation(APowerLine* PowerLine)
{
	ConnectedPowerLines.Remove(PowerLine);
	PowerLine->ClearPowerFeeder();
	
	return true;
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
	if (DoesIncreasePopulationCapacity())
	{
		GetStrategyGameState()->IncreasePopulationCapacity(AdditionalPopulationCapacity);
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

void AStructure::AssignWorkers(ECitizenType WorkerType, int32 Amount)
{
	if (Amount <= 0) return;
	
	if (IsWorkerCapacityFull())
	{
		GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Red, DisplayName + ": Worker Capacity is full.");
		return;
	}
	
	if (!bAllowScientistEmployment && WorkerType == ECitizenType::Scientist)
	{
		GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Red, DisplayName + "Scientists aren't permitted to work here.");
		return;
	}
	if (!bAllowWorkerEmployment && WorkerType == ECitizenType::Worker)
	{
		GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Red, DisplayName + "Workers aren't permitted to work here.");
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

void AStructure::AddMaxWorkers(ECitizenType WorkerType)
{
	AssignWorkers(WorkerType, GetAvailableWorkersSlots());
}

void AStructure::RemoveWorkers(ECitizenType WorkerType, int32 Amount)
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

void AStructure::RemoveAllWorkers(ECitizenType WorkerType)
{
	switch (WorkerType) {
	case ECitizenType::Worker:
		RemoveWorkers(ECitizenType::Worker, GetWorkerCount(ECitizenType::Worker));
		break;
	case ECitizenType::Scientist:
		RemoveWorkers(ECitizenType::Scientist, GetWorkerCount(ECitizenType::Scientist));
		break;
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

	FVector CameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(StructureText->GetComponentLocation(), CameraLocation);
	StructureText->SetWorldRotation(LookAtRotation);
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

