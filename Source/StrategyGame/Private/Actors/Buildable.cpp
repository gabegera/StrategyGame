// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Buildable.h"

#include "Actors/BuildExclusionZone.h"
#include "Actors/ResourceNode.h"
#include "Components/ArrowComponent.h"
#include "Pawns/RTSCamera.h"

// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Structure Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("SelectableObject");
	StaticMesh->SetGenerateOverlapEvents(false);
	
	BuildableBounds = CreateDefaultSubobject<UBoxComponent>("Buildable Bounds");
    BuildableBounds->SetupAttachment(StaticMesh);
    BuildableBounds->SetBoxExtent(FVector(1000.0f, 1000.0f, 1000.0f));
    BuildableBounds->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f));
    BuildableBounds->SetCollisionProfileName("Trigger");
    BuildableBounds->SetGenerateOverlapEvents(true);
	BuildableBounds->SetLineThickness(20.0f);
	
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>("Forward Arrow");
	ForwardArrow->SetupAttachment(RootComponent);
	ForwardArrow->SetArrowSize(15.0f);
	ForwardArrow->SetScreenSize(15.0f);
}

void ABuildable::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BuildableBounds->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	BuildableBounds->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);

	BuildableBounds->SetBoxExtent(FVector(BuildableBounds->GetUnscaledBoxExtent().X - 1, BuildableBounds->GetUnscaledBoxExtent().Y - 1, BuildableBounds->GetUnscaledBoxExtent().Z - 1));
}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();

	StructureMaterial = StaticMesh->GetMaterial(0);

	UpdateBuildMaterials();

	if (IsBeingCreated())
	{
		BuildableBounds->SetHiddenInGame(false);
	}
}

void ABuildable::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABuildable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABuildExclusionZone::StaticClass()) ||
		OtherActor->IsA(ABuildable::StaticClass()))
	{
		OverlappingExclusionZones.AddUnique(OtherActor);
	}

	UpdateBuildMaterials();
}

void ABuildable::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingExclusionZones.Remove(OtherActor);
	OverlappingResourceNodes.Remove(OtherActor);

	UpdateBuildMaterials();
}

bool ABuildable::Select_Implementation(ARTSCamera* SelectInstigator)
{
	BuildableSelectedDelegate.Broadcast(this);
	
	return true;
}

bool ABuildable::Recycle_Implementation(ARTSCamera* RecycleInstigator)
{
	BuildableRecycledDelegate.Broadcast(this);
	Recycle();
	return true;
}

void ABuildable::BeginConstruction()
{
	ConsumeConstructionResources();
	GetWorldTimerManager().SetTimer(ConstructionTimer, this, &ABuildable::CompleteConstruction, ConstructionTime, false);
	StructureMode = EBuildableMode::UnderConstruction;
	UpdateBuildMaterials();
}

void ABuildable::CancelConstruction()
{
	GetWorldTimerManager().ClearTimer(ConstructionTimer);
	RefundConstructionMaterials();
	
	Destroy();
}

void ABuildable::ConsumeConstructionResources()
{
	for (auto Resource : ConstructionCost)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->ConsumeResources(ResourceType, ResourceAmount);
	}
}

void ABuildable::RefundConstructionMaterials()
{
	for (auto Resource : ConstructionCost)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->AddResources(ResourceType, ResourceAmount);
	}
}

void ABuildable::CompleteConstruction()
{
	StructureMode = EBuildableMode::Complete;
	BuildableBounds->SetHiddenInGame(true);
	UpdateBuildMaterials();
}

void ABuildable::Recycle()
{
	RefundConstructionMaterials();
	
	Destroy();
}

void ABuildable::UpdateBuildMaterials()
{
	if (IsUnderConstruction())
	{
		if (StaticMesh->GetMaterial(0) != IsBuildingMaterial) StaticMesh->SetMaterial(0, IsBuildingMaterial);
		return;
	}
	if (IsCompleted())
	{
		if (StaticMesh->GetMaterial(0) != StructureMaterial) StaticMesh->SetMaterial(0, StructureMaterial);
		return;
	}

	if (IsBuildingPermitted())
	{
		if (StaticMesh->GetMaterial(0) != CanBuildMaterial) StaticMesh->SetMaterial(0, CanBuildMaterial);
	}
	else
	{
		if (StaticMesh->GetMaterial(0) != CanNotBuildMaterial) StaticMesh->SetMaterial(0, CanNotBuildMaterial);
	}
}

// Called every frame
void ABuildable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABuildable::IsBuildingPermitted()
{
	if (!HaveEnoughResourcesToBuild() && IsBeingCreated())
	{
		GEngine->AddOnScreenDebugMessage(802, 3.0f, FColor::Red, "Not enough materials to build " + GetDisplayName());
		return false;
	}
	
	if (IsOverlappingBuildExclusionZone())
	{
		GEngine->AddOnScreenDebugMessage(800, 3.0f, FColor::Red, GetDisplayName() + " is overlapping Build Exclusion Zone.");
		return false;
	}

	return true;
}

bool ABuildable::HaveEnoughResourcesToBuild()
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

