// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildableStructure.h"

#include "Actors/BuildExclusionZone.h"
#include "Actors/ResourceNode.h"
#include "Components/ArrowComponent.h"

// Sets default values
ABuildableStructure::ABuildableStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Structure Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetGenerateOverlapEvents(false);
	
	StructureBounds = CreateDefaultSubobject<UBoxComponent>("Structure Bounds");
    StructureBounds->SetupAttachment(StaticMesh);
    StructureBounds->SetBoxExtent(FVector(1000.0f, 1000.0f, 1000.0f));
    StructureBounds->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f));
    StructureBounds->SetCollisionProfileName("Trigger");
    StructureBounds->SetGenerateOverlapEvents(true);
	StructureBounds->SetLineThickness(20.0f);
	
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>("Forward Arrow");
	ForwardArrow->SetupAttachment(RootComponent);
	ForwardArrow->SetArrowSize(15.0f);
	ForwardArrow->SetScreenSize(15.0f);
}

void ABuildableStructure::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StructureBounds->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	StructureBounds->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

	StructureBounds->SetBoxExtent(FVector(StructureBounds->GetUnscaledBoxExtent().X - 1, StructureBounds->GetUnscaledBoxExtent().Y - 1, StructureBounds->GetUnscaledBoxExtent().Z - 1));
}

// Called when the game starts or when spawned
void ABuildableStructure::BeginPlay()
{
	Super::BeginPlay();

	UpdateBuildMaterials();

	if (IsBeingCreated())
	{
		StructureBounds->SetHiddenInGame(false);
	}
}

void ABuildableStructure::BeginDestroy()
{
	Super::BeginDestroy();

	
}

void ABuildableStructure::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABuildExclusionZone::StaticClass()) ||
		OtherActor->IsA(ABuildableStructure::StaticClass()))
	{
		OverlappingExclusionZones.AddUnique(OtherActor);
	}

	if (OtherActor->IsA(AResourceNode::StaticClass()) &&
		bConsumesResourceFromNearbyNode)
	{
		AResourceNode* Resource = Cast<AResourceNode>(OtherActor);
		if (ResourcesToConsumePerSecond.Contains(Resource->GetResourceType()))
		{
			OverlappingResourceNodes.Add(Resource);
		}
	}

	UpdateBuildMaterials();
}

void ABuildableStructure::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingExclusionZones.Remove(OtherActor);
	OverlappingResourceNodes.Remove(OtherActor);

	UpdateBuildMaterials();
}

bool ABuildableStructure::Select_Implementation(ARTSCamera* SelectInstigator)
{
	OnStructureSelectedDelegate.Broadcast(this);
	
	return true;
}

bool ABuildableStructure::BeginConstruction()
{
	if (HaveEnoughResourcesToBuild())
	{
		ConsumeConstructionResources();
		GetWorldTimerManager().SetTimer(ConstructionTimer, this, &ABuildableStructure::CompleteConstruction, ConstructionTime, false);
		StructureMode = EStructureMode::UnderConstruction;
		UpdateBuildMaterials();

		if (bConsumesResourceFromNearbyNode)
		{
			TargetResourceNode = FindClosestResourceNode();
			if (TargetResourceNode) TargetResourceNode->SetAssignedExtractor(this);
		}
		
		return true;
	}

	CancelConstruction();
	return false;
}

void ABuildableStructure::CancelConstruction()
{
	GetWorldTimerManager().ClearTimer(ConstructionTimer);
	RefundConstructionMaterials();
	
	Destroy();
}

void ABuildableStructure::ConsumeConstructionResources()
{
	for (auto Resource : ConstructionCost)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->ConsumeResources(ResourceType, ResourceAmount);
	}
}

void ABuildableStructure::RefundConstructionMaterials()
{
	for (auto Resource : ConstructionCost)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->AddResources(ResourceType, ResourceAmount);
	}
}

void ABuildableStructure::CompleteConstruction()
{
	StructureMode = EStructureMode::Complete;
	StructureBounds->SetHiddenInGame(true);
	ActivateStructureEffects();
	UpdateBuildMaterials();
}

void ABuildableStructure::UpdateBuildMaterials()
{
	if (IsUnderConstruction())
	{
		if (StaticMesh->GetMaterial(0) != IsBuildingMaterial) StaticMesh->SetMaterial(0, IsBuildingMaterial);
		return;
	}
	if (IsCompleted())
	{
		if (StaticMesh->GetMaterial(0) != CanBuildMaterial) StaticMesh->SetMaterial(0, CanBuildMaterial);
		return;
	}
	
	if (IsOverlappingBuildExclusionZone())
	{
		if (StaticMesh->GetMaterial(0) != CanNotBuildMaterial) StaticMesh->SetMaterial(0, CanNotBuildMaterial);
	}
	else if (bConsumesResourceFromNearbyNode && !IsOverlappingResourceNode())
	{
		if (StaticMesh->GetMaterial(0) != CanNotBuildMaterial) StaticMesh->SetMaterial(0, CanNotBuildMaterial);
	}
	else if (bConsumesResourceFromNearbyNode && FindClosestResourceNode() && FindClosestResourceNode()->GetAssignedExtractor())
	{
		if (StaticMesh->GetMaterial(0) != CanNotBuildMaterial) StaticMesh->SetMaterial(0, CanNotBuildMaterial);
	}
	else
	{
		if (StaticMesh->GetMaterial(0) != CanBuildMaterial) StaticMesh->SetMaterial(0, CanBuildMaterial);
	}
}

void ABuildableStructure::ActivateStructureEffects()
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

AResourceNode* ABuildableStructure::FindClosestResourceNode()
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

void ABuildableStructure::BeginGeneratingResources()
{
	GetWorldTimerManager().SetTimer(ResourceGenerationTimer, this, &ABuildableStructure::GenerateResources, 1.0f, true);
}

void ABuildableStructure::GenerateResources()
{
	for (auto Resource : ResourcesToGeneratePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->AddResources(ResourceType, ResourceAmount);
	}
}

void ABuildableStructure::BeginConsumingResources()
{
	GetWorldTimerManager().SetTimer(ResourceConsumptionTimer, this, &ABuildableStructure::ConsumeResources, 1.0f, true);
}

void ABuildableStructure::ConsumeResources()
{
	for (auto Resource : ResourcesToConsumePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->ConsumeResources(ResourceType, ResourceAmount);
	}
}

void ABuildableStructure::BeginDrainingResourceFromNode()
{
	GetWorldTimerManager().SetTimer(ResourceDrainingTimer, this, &ABuildableStructure::DrainResourceFromNode, 1.0f, true);
}

void ABuildableStructure::DrainResourceFromNode()
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

// Called every frame
void ABuildableStructure::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AStrategyGameState* ABuildableStructure::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

bool ABuildableStructure::HaveEnoughResourcesToBuild()
{
	for (TPair ResourceCost : ConstructionCost)
	{
		EResourceType ResourceType = ResourceCost.Key;
		int32 AmountNeeded = ResourceCost.Value;
		
		if (GetStrategyGameState()->GetResourceAmount(ResourceType) < AmountNeeded)
		{
			return false;
		}
	}

	return true;
}

