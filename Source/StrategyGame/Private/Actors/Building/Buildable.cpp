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

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Structure Mesh");
	StaticMeshComponent->SetupAttachment(SceneComponent);
	StaticMeshComponent->SetCollisionProfileName("SelectableObject");
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	
	BuildingBounds = CreateDefaultSubobject<UBoxComponent>("Building Bounds");
    BuildingBounds->SetupAttachment(StaticMeshComponent);
    BuildingBounds->SetCollisionProfileName("OverlapAll");
    BuildingBounds->SetGenerateOverlapEvents(true);
	BuildingBounds->SetLineThickness(20.0f);

	ForwardIdentifierMesh = CreateDefaultSubobject<UStaticMeshComponent>("Forward Arrow");
    ForwardIdentifierMesh->SetupAttachment(SceneComponent);
    ForwardIdentifierMesh->SetHiddenInGame(false);

	BuildableStateChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnBuildableStateChanged);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> CanBuildMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_CanBuild.MI_CanBuild"));
	if (CanBuildMaterialFinder.Succeeded()) CanBuildMaterial = CanBuildMaterialFinder.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> CanNotBuildMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_CannotBuild.MI_CannotBuild"));
	if (CanBuildMaterialFinder.Succeeded()) CanNotBuildMaterial = CanNotBuildMaterialFinder.Object;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> IsBuildingMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_IsBuilding.MI_IsBuilding"));
	if (CanBuildMaterialFinder.Succeeded()) IsBuildingMaterial = IsBuildingMaterialFinder.Object;
}

// Called when the game starts or when spawned
void ABuildable::BeginPlay()
{
	Super::BeginPlay();

	StructureMaterial = StaticMeshComponent->GetMaterial(0);

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

	// Attempts to search for the set snapping grid size and resizes the building bounds based upon it and the static mesh size.
	FVector StaticMeshBounds = FVector::ZeroVector;
	if (StaticMeshComponent->GetStaticMesh()) StaticMeshBounds = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
	
	int32 HalfSnappingSize = 200;
	FString GameModePath = TEXT("/Game/Blueprints/GameModes/BP_StrategyGameModeBase.BP_StrategyGameModeBase_C");
	if (const TSubclassOf<AStrategyGameModeBase> GameModeClass = StaticLoadClass(AStrategyGameModeBase::StaticClass(), nullptr, *GameModePath))
	{
		HalfSnappingSize = GameModeClass.GetDefaultObject()->GetSnappingSize() / 2;
	}

	if (StaticMeshBounds == FVector::ZeroVector) StaticMeshBounds = FVector(HalfSnappingSize, HalfSnappingSize, HalfSnappingSize);
	FVector SnappedBounds = FVector(FMath::CeilToInt32(StaticMeshBounds.X / HalfSnappingSize) * HalfSnappingSize, FMath::CeilToInt32(StaticMeshBounds.Y / HalfSnappingSize) * HalfSnappingSize, StaticMeshBounds.Z + 100);
	BuildingBounds->SetBoxExtent(SnappedBounds);
	BuildingBounds->SetRelativeLocation(FVector(0.0f, 0.0f, BuildingBounds->GetScaledBoxExtent().Z));

	// If the bounds of the buildable are an odd number of grids, then add a snapping offset to line up with the snapping grid properly.
	if (static_cast<int32>(BuildingBounds->GetUnscaledBoxExtent().X) / HalfSnappingSize % 2 !=0)
	{
		SnappingOffset.X = HalfSnappingSize;
	}
	if (static_cast<int32>(BuildingBounds->GetUnscaledBoxExtent().Y) / HalfSnappingSize % 2 !=0)
	{
		SnappingOffset.Y = HalfSnappingSize;
	}

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

void ABuildable::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingExclusionZones.Remove(OtherActor);

	UpdateBuildMaterials();
}

void ABuildable::OnBuildableStateChanged(ABuildable* Buildable, EBuildableState NewBuildableState)
{
	BP_OnBuildableStateChanged(Buildable, NewBuildableState);

	switch (NewBuildableState)
	{
	case EBuildableState::BeingCreated:
		BuildingBounds->SetHiddenInGame(false);
		break;
	case EBuildableState::UnderConstruction:
		BuildingBounds->SetHiddenInGame(false);
		break;
	case EBuildableState::ConstructionComplete:
		BuildingBounds->SetHiddenInGame(true);
		break;
	}
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
	SetBuildableState(EBuildableState::UnderConstruction);
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
	SetBuildableState(EBuildableState::ConstructionComplete);
	UpdateBuildMaterials();
}

void ABuildable::Recycle()
{
	RefundConstructionMaterials();
	
	Destroy();
}

void ABuildable::UpdateBuildMaterials()
{
	ensureMsgf(CanBuildMaterial, TEXT("ABuildable::UpdateBuildMaterials CanBuildMaterial is not set"));
	ensureMsgf(CanNotBuildMaterial, TEXT("ABuildable::UpdateBuildMaterials CanNotBuild is not set"));
	ensureMsgf(IsBuildingMaterial, TEXT("ABuildable::UpdateBuildMaterials IsBuilding is not set"));
	
	if (IsUnderConstruction())
	{
		if (!CanBuildMaterial) return;
		if (StaticMeshComponent->GetMaterial(0) != IsBuildingMaterial) StaticMeshComponent->SetMaterial(0, IsBuildingMaterial);
		return;
	}
	if (IsConstructionComplete())
	{
		if (!StructureMaterial) return;
		if (StaticMeshComponent->GetMaterial(0) != StructureMaterial) StaticMeshComponent->SetMaterial(0, StructureMaterial);
		return;
	}

	if (IsBuildingPermitted())
	{
		if (!CanBuildMaterial) return;
		if (StaticMeshComponent->GetMaterial(0) != CanBuildMaterial) StaticMeshComponent->SetMaterial(0, CanBuildMaterial);
	}
	else
	{
		if (!CanNotBuildMaterial) return;
		if (StaticMeshComponent->GetMaterial(0) != CanNotBuildMaterial) StaticMeshComponent->SetMaterial(0, CanNotBuildMaterial);
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

