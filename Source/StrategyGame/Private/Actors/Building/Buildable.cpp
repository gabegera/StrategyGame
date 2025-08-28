// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Buildable.h"

#include "Actors/Building/BuildExclusionZone.h"
#include "Actors/ResourceNode.h"
#include "Actors/Building/Road.h"
#include "Actors/Building/Structure.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Pawns/RTSCamera.h"

// Sets default values
ABuildable::ABuildable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Structure Mesh");
	StaticMesh->SetupAttachment(SceneComponent);
	StaticMesh->SetCollisionProfileName("SelectableObject");
	StaticMesh->SetGenerateOverlapEvents(true);
	
	BuildingBounds = CreateDefaultSubobject<UBoxComponent>("Building Bounds");
    BuildingBounds->SetupAttachment(StaticMesh);
    BuildingBounds->SetBoxExtent(FVector(1000.0f, 1000.0f, 1000.0f));
    BuildingBounds->SetRelativeLocation(FVector(0.0f, 0.0f, 1000.0f));
    BuildingBounds->SetCollisionProfileName("OverlapAll");
    BuildingBounds->SetGenerateOverlapEvents(true);
	BuildingBounds->SetLineThickness(20.0f);

	ForwardIdentifierMesh = CreateDefaultSubobject<UStaticMeshComponent>("Forward Arrow");
    ForwardIdentifierMesh->SetupAttachment(SceneComponent);
    ForwardIdentifierMesh->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();

	StructureMaterial = StaticMesh->GetMaterial(0);

	UpdateBuildMaterials();

	if (IsBeingCreated())
	{
		BuildingBounds->SetHiddenInGame(false);
	}

	BuildingBounds->SetBoxExtent(FVector(BuildingBounds->GetScaledBoxExtent().X - 5, BuildingBounds->GetScaledBoxExtent().Y - 5, BuildingBounds->GetScaledBoxExtent().Z - 5));
	BuildingBounds->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
	BuildingBounds->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnOverlapEnd);
}

void ABuildable::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	BuildingBounds->SetRelativeLocation(FVector(0.0f, 0.0f, BuildingBounds->GetScaledBoxExtent().Z));

	ForwardIdentifierMesh->SetRelativeLocation(ForwardIdentifierMesh->GetForwardVector() * BuildingBounds->GetScaledBoxExtent().X + FVector::UpVector * BuildingBounds->GetScaledBoxExtent().Z);
}

void ABuildable::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABuildable::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	UpdateBuildMaterials();
}

bool ABuildable::GetIsConstructionComplete_Implementation()
{
	return IsConstructionComplete();
}

bool ABuildable::Recycle_Implementation(ARTSCamera* RecycleInstigator)
{
	Recycle();
	return true;
}

void ABuildable::MoveBuilding(FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void ABuildable::PlaceBuilding()
{
	if (!IsBuildingPermitted()) return;
	
	ABuildable* NewStructure = GetWorld()->SpawnActor<ABuildable>(GetClass(), GetActorTransform());
	NewStructure->BeginConstruction();
}

void ABuildable::BeginConstruction()
{
	ConsumeConstructionResources();
	
	if (ConstructionTime == 0)
	{
		CompleteConstruction();
		return;
	}
	
	GetWorldTimerManager().SetTimer(ConstructionTimer, this, &ABuildable::CompleteConstruction, ConstructionTime, false);
	StructureMode = EBuildableMode::UnderConstruction;
	UpdateBuildMaterials();
	ForwardIdentifierMesh->SetHiddenInGame(true);
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
	StructureMode = EBuildableMode::ConstructionComplete;
	BuildingBounds->SetHiddenInGame(true);
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
	if (IsConstructionComplete())
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
		GEngine->AddOnScreenDebugMessage(801, 3.0f, FColor::Red, "Not enough materials to build " + GetDisplayName());
		return false;
	}
	
	if (IsOverlappingBuildExclusionZone())
	{
		GEngine->AddOnScreenDebugMessage(802, 3.0f, FColor::Red, GetDisplayName() + " is overlapping Build Exclusion Zone.");
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

