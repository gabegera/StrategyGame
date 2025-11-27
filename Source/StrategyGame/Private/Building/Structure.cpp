// Fill out your copyright notice in the Description page of Project Settings.


#include "Building/Structure.h"

#include "Building/BuildExclusionZone.h"
#include "ResourceNode.h"
#include "Components/ArrowComponent.h"
#include "Components/HousingComponent.h"
#include "Components/LookAtCameraTextRenderComponent.h"
#include "Components/ResourceGenerationComponent.h"
#include "Components/ResourceHarvestingComponent.h"
#include "Components/ResourceStorageComponent.h"
#include "Components/WorkersComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/StrategyGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavAreas/NavArea_Obstacle.h"
#include "Player/RTSCamera.h"

// Sets default values
AStructure::AStructure()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Structure Mesh");
	StaticMeshComponent->SetupAttachment(SceneComponent);
	StaticMeshComponent->SetCollisionProfileName("Selectable");
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	
	BuildingBounds = CreateDefaultSubobject<UBoxComponent>("Building Bounds");
    BuildingBounds->SetupAttachment(StaticMeshComponent);
    BuildingBounds->SetCollisionProfileName("OverlapAll");
    BuildingBounds->SetGenerateOverlapEvents(true);
	BuildingBounds->SetLineThickness(20.0f);
	BuildingBounds->bDynamicObstacle = true;
	BuildingBounds->SetAreaClassOverride(UNavArea_Obstacle::StaticClass());
	BuildingBounds->SetCanEverAffectNavigation(true);

	StructureEntranceArrow = CreateDefaultSubobject<UArrowComponent>("Entrance");
	StructureEntranceArrow->SetupAttachment(StaticMeshComponent);
	StructureEntranceArrow->SetArrowSize(2.5f);
	StructureEntranceArrow->SetArrowLength(20.0f);
	StructureEntranceArrow->SetRelativeLocation(FVector(250.0f, 0.0f, 0.0f));

	LookAtCameraTextRenderComponent = CreateDefaultSubobject<ULookAtCameraTextRenderComponent>("Structure Name");
	LookAtCameraTextRenderComponent->SetupAttachment(BuildingBounds);
	LookAtCameraTextRenderComponent->SetWorldSize(256.0f);
	LookAtCameraTextRenderComponent->SetHorizontalAlignment(EHTA_Center);
	
	ConstructorHelpers::FObjectFinder<UMaterialInstance> CanBuildMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_CanBuild.MI_CanBuild"));
	if (CanBuildMaterialFinder.Succeeded())
	{
	 	CanBuildMaterial = CanBuildMaterialFinder.Object;
		
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> CanNotBuildMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_CannotBuild.MI_CannotBuild"));
	if (CanBuildMaterialFinder.Succeeded())
	{
	 	CanNotBuildMaterial = CanNotBuildMaterialFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInstance> IsBuildingMaterialFinder(TEXT("/Game/Assets/Structures/ConstructionMaterials/MI_IsBuilding.MI_IsBuilding"));
	if (CanBuildMaterialFinder.Succeeded())
	{
	 	IsBuildingMaterial = IsBuildingMaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void AStructure::BeginPlay()
{
	Super::BeginPlay();

	LoadRandomMesh();

	DefaultMaterial = StaticMeshComponent->GetMaterial(0);

	if (IsBeingPlaced())
	{
		BuildingBounds->SetHiddenInGame(false);
	}

	BuildingBounds->SetBoxExtent(FVector(BuildingBounds->GetUnscaledBoxExtent().X - 5, BuildingBounds->GetUnscaledBoxExtent().Y - 5, BuildingBounds->GetUnscaledBoxExtent().Z - 5));
	
	BuildingBounds->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlapBegin);
	BuildingBounds->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnOverlapEnd);

	UpdateBuildMaterials();
}

void AStructure::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!MeshVariety.IsEmpty()) 
	{
		TSoftObjectPtr<UStaticMesh> FirstMesh = MeshVariety.Array()[0];

		if (FirstMesh.IsValid())
		{
			StaticMeshComponent->SetStaticMesh(FirstMesh.Get());
		}
		else
		{
			StaticMeshComponent->SetStaticMesh(FirstMesh.LoadSynchronous());
		}
	}
	
	// Attempts to search for the set snapping grid size and resizes the building bounds based upon it and the static mesh size.
	FVector StaticMeshBounds = FVector::ZeroVector;
	if (StaticMeshComponent->GetStaticMesh()) StaticMeshBounds = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
	
	int32 HalfSnappingSize = 0;
	FString GameInstancePath = TEXT("/Game/Blueprints/Game/BP_StrategyGameInstance.BP_StrategyGameInstance_C");
	if (const TSubclassOf<UStrategyGameInstance> GameInstanceClass = StaticLoadClass(UStrategyGameInstance::StaticClass(), nullptr, *GameInstancePath))
	{
		HalfSnappingSize = GameInstanceClass.GetDefaultObject()->GetGridSize() / 2;
	}
	
	if (StaticMeshBounds == FVector::ZeroVector) StaticMeshBounds = FVector(HalfSnappingSize, HalfSnappingSize, HalfSnappingSize);
	FVector SnappedBounds = FVector(FMath::CeilToInt32(StaticMeshBounds.X / HalfSnappingSize) * HalfSnappingSize, FMath::CeilToInt32(StaticMeshBounds.Y / HalfSnappingSize) * HalfSnappingSize, StaticMeshBounds.Z + 100);
	BuildingBounds->SetBoxExtent(SnappedBounds);
	BuildingBounds->SetRelativeLocation(FVector(0.0f, 0.0f, BuildingBounds->GetScaledBoxExtent().Z));
	
	// If the bounds of the Structure are an odd number of grids, then add a snapping offset to line up with the snapping grid properly.
	if (static_cast<int32>(BuildingBounds->GetUnscaledBoxExtent().X) / HalfSnappingSize % 2 != 0)
	{
		SnappingOffset.X = HalfSnappingSize;
	}
	if (static_cast<int32>(BuildingBounds->GetUnscaledBoxExtent().Y) / HalfSnappingSize % 2 != 0)
	{
		SnappingOffset.Y = HalfSnappingSize;
	}

	LookAtCameraTextRenderComponent->SetRelativeLocation(FVector::UpVector * BuildingBounds->GetScaledBoxExtent().Z);
	LookAtCameraTextRenderComponent->SetText(StructureName);

	const FRotator EntranceLookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetEntranceLocation(), SceneComponent->GetComponentLocation());
	StructureEntranceArrow->SetWorldRotation(EntranceLookAtRotation);
}

void AStructure::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABuildExclusionZone::StaticClass()) ||
		OtherActor->IsA(AStructure::StaticClass()))
	{
		OverlappingExclusionZones.AddUnique(OtherActor);
	}
	
	UpdateBuildMaterials();
}

void AStructure::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingExclusionZones.Remove(OtherActor);

	UpdateBuildMaterials();
}

void AStructure::OnStructureStateChanged(AStructure* Structure, EStructureState NewStructureState)
{
	switch (NewStructureState)
	{
	case EStructureState::BeingPlaced:
		BuildingBounds->SetHiddenInGame(false);
		break;
	case EStructureState::UnderConstruction:
		BuildingBounds->SetHiddenInGame(false);
		break;
	case EStructureState::ConstructionComplete:
		BuildingBounds->SetHiddenInGame(true);
		break;
	}
}

bool AStructure::TrySelect(ARTSCamera* SelectInstigator)
{
	GetGameInstance<UStrategyGameInstance>()->OnStructureSelected.Broadcast(this);
	return true;
}

bool AStructure::TryRecycle(ARTSCamera* DestroyInstigator)
{
	Recycle();
	return true;
}

bool AStructure::DoesIncreaseStorage()
{
	const UResourceStorageComponent* ResourceStorageComponent = GetComponentByClass<UResourceStorageComponent>();
	return ResourceStorageComponent && !ResourceStorageComponent->GetStorageResources().IsEmpty();
}

bool AStructure::DoesGenerateResources()
{
	const UResourceGenerationComponent* ResourceGenerationComponent = GetComponentByClass<UResourceGenerationComponent>();
	return ResourceGenerationComponent && !ResourceGenerationComponent->GetGeneratedResources().IsEmpty();
}

bool AStructure::DoesHarvestResources()
{
	const UResourceHarvestingComponent* ResourceHarvestingComponent = GetComponentByClass<UResourceHarvestingComponent>();
	return ResourceHarvestingComponent && !ResourceHarvestingComponent->GetHarvestingResources().IsEmpty();
}

bool AStructure::DoesProvideHousing()
{
	const UHousingComponent* HousingComponent = GetComponentByClass<UHousingComponent>();
	return HousingComponent && HousingComponent->GetHousingCapacity() > 0;
}

bool AStructure::DoesRequireWorkers()
{
	const UWorkersComponent* WorkersComponent = GetComponentByClass<UWorkersComponent>();
	return WorkersComponent && WorkersComponent->GetMaxNumOfWorkers() > 0;
}

void AStructure::MoveBuilding(const FVector NewLocation)
{
	SetActorLocation(NewLocation);
}

void AStructure::PlaceBuilding()
{
	if (!IsBuildingPermitted()) return;
	
	AStructure* NewStructure = GetWorld()->SpawnActor<AStructure>(GetClass(), GetActorTransform());
	NewStructure->BeginConstruction();
}

void AStructure::BeginConstruction()
{
	ConsumeConstructionResources();
	
	if (TimeToCompleteConstruction == 0)
	{
		CompleteConstruction();
		return;
	}

	GetWorldTimerManager().SetTimer(ConstructionTimer, this, &AStructure::CompleteConstruction, TimeToCompleteConstruction);
	
	SetStructureState(EStructureState::UnderConstruction);
	UpdateBuildMaterials();
}

void AStructure::CancelConstruction()
{
	GetWorldTimerManager().ClearTimer(ConstructionTimer);
	RefundConstructionMaterials();
	
	Destroy();
}

void AStructure::ConsumeConstructionResources()
{
	for (auto Resource : ConstructionCost)
	{
		UResourceDataAsset* ResourceData = Resource.Key;
		int32 ResourceAmount = Resource.Value;

		GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->ConsumeResources(ResourceData, ResourceAmount);
	}
}

void AStructure::RefundConstructionMaterials()
{
	for (auto Resource : ConstructionCost)
	{
		UResourceDataAsset* ResourceData = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->AddResources(ResourceData, ResourceAmount);
	}
}

void AStructure::CompleteConstruction()
{
	SetStructureState(EStructureState::ConstructionComplete);
	UpdateBuildMaterials();
	GetGameInstance<UStrategyGameInstance>()->OnStructureBuilt.Broadcast(this);
}

void AStructure::DestroyStructure()
{
	GetGameInstance<UStrategyGameInstance>()->OnStructureDestroyed.Broadcast(this);
	Destroy();
}

void AStructure::Recycle()
{
	RefundConstructionMaterials();
	DestroyStructure();
}

void AStructure::UpdateBuildMaterials()
{
	ensureMsgf(CanBuildMaterial, TEXT("%s AStructure::UpdateBuildMaterials CanBuildMaterial is not set"), *GetName());
	ensureMsgf(CanNotBuildMaterial, TEXT("%s AStructure::UpdateBuildMaterials CanNotBuildMaterial is not set"), *GetName());
	ensureMsgf(IsBuildingMaterial, TEXT("%s AStructure::UpdateBuildMaterials IsBuildingMaterial is not set"), *GetName());

	TArray<UStaticMeshComponent*> StaticMeshComponents;
	GetComponents<UStaticMeshComponent>(StaticMeshComponents);
	
	if (IsUnderConstruction())
	{
		if (!IsBuildingMaterial) return;
		
		for (auto StaticMesh : StaticMeshComponents)
		{
			StaticMesh->SetMaterial(0, IsBuildingMaterial);
		}
		return;
	}
	if (IsConstructionComplete())
	{
		if (!DefaultMaterial) return;
		
		for (auto StaticMesh : StaticMeshComponents)
		{
			StaticMesh->SetMaterial(0, DefaultMaterial);
		}
		return;
	}

	if (IsBuildingPermitted())
	{
		if (!CanBuildMaterial) return;
		
		for (auto StaticMesh : StaticMeshComponents)
		{
			StaticMesh->SetMaterial(0, CanBuildMaterial);
		}
	}
	else
	{
		if (!CanNotBuildMaterial) return;
		
		for (auto StaticMesh : StaticMeshComponents)
		{
			StaticMesh->SetMaterial(0, CanNotBuildMaterial);
		}
	}
}

void AStructure::LoadRandomMesh() const
{
	if (MeshVariety.IsEmpty()) return;

	const TSoftObjectPtr<UStaticMesh> RandomMesh = MeshVariety.Array()[FMath::RandRange(0, MeshVariety.Num() - 1)];

	if (RandomMesh.IsValid())
	{
		StaticMeshComponent->SetStaticMesh(RandomMesh.Get());
		return;
	}
	
	UAssetManager::GetStreamableManager().RequestAsyncLoad(RandomMesh.ToSoftObjectPath(), FStreamableDelegate::CreateUObject(this, &ThisClass::OnMeshLoaded, RandomMesh));
}

void AStructure::OnMeshLoaded(const TSoftObjectPtr<UStaticMesh> LoadedMesh) const
{
	if (!LoadedMesh.IsValid()) return;

	StaticMeshComponent->SetStaticMesh(LoadedMesh.Get());
}

FVector AStructure::GetEntranceLocation() const
{
	return StructureEntranceArrow->GetComponentLocation();
}

EStructureState AStructure::GetStructureState() const
{
	return StructureState;
}

EStructureState AStructure::SetStructureState(EStructureState NewMode)
{
	StructureState = NewMode;
	UpdateBuildMaterials();

	return StructureState;
}

bool AStructure::IsBuildingPermitted()
{
	if (!HaveEnoughResourcesToBuild() && IsBeingPlaced())
	{
		GEngine->AddOnScreenDebugMessage(801, 3.0f, FColor::Red, "Not enough materials to build " + StructureName.ToString());
		return false;
	}
	
	if (IsOverlappingBuildExclusionZone())
	{
		GEngine->AddOnScreenDebugMessage(802, 3.0f, FColor::Red, StructureName.ToString() + " is overlapping Build Exclusion Zone.");
		return false;
	}

	return true;
}

bool AStructure::HaveEnoughResourcesToBuild()
{
	for (TPair ResourceCost : ConstructionCost)
	{
		UResourceDataAsset* ResourceData = ResourceCost.Key;
		int32 AmountNeeded = ResourceCost.Value;
		
		if (GetGameInstance()->GetSubsystem<UResourcesSubsystem>()->GetResourceAmountInt(ResourceData) < AmountNeeded)
		{
			return false;
		}
	}

	return true;
}

