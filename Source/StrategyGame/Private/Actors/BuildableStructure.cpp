// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BuildableStructure.h"

#include "Actors/BuildExclusionZone.h"
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

	StaticMesh->SetMaterial(0, CanBuildMaterial);

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

	if (IsOverlappingBuildExclusionZone())
	{
		if (!IsUnderConstruction() && !IsCompleted())
		{
			StaticMesh->SetMaterial(0, CanNotBuildMaterial);
		}
	}
}

void ABuildableStructure::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingExclusionZones.Remove(OtherActor);

	if (!IsOverlappingBuildExclusionZone() && !IsUnderConstruction() && !IsCompleted())
	{
		StaticMesh->SetMaterial(0, CanBuildMaterial);
	}
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
		ConsumeResources(ConstructionCost);
		GetWorldTimerManager().SetTimer(ConstructionTimer, this, &ABuildableStructure::CompleteConstruction, ConstructionTime, false);
		StructureMode = EStructureMode::UnderConstruction;
		StaticMesh->SetMaterial(0, IsBuildingMaterial);
		return true;
	}

	CancelConstruction();
	return false;
}

void ABuildableStructure::CancelConstruction()
{
	GetWorldTimerManager().ClearTimer(ConstructionTimer);
	Destroy();
}

void ABuildableStructure::CompleteConstruction()
{
	StructureMode = EStructureMode::Complete;
	StaticMesh->SetMaterial(0, CanBuildMaterial);
	StructureBounds->SetHiddenInGame(true);
	ActivateStructureEffects();
}

void ABuildableStructure::ActivateStructureEffects()
{
	TMap<EResourceType, int32> ResourcesToGeneratePerSecond;

	TMap<EResourceType, int32> ResourcesToConsumePerSecond;
	
	for (auto Effect : StructureEffects)
	{
		EStructureEffect EffectType = Effect.Key;
		int32 EffectValue = Effect.Value;

		switch (EffectType)
		{
		case EStructureEffect::IncreasesMetalPerSecond:
			ResourcesToGeneratePerSecond.Add(EResourceType::Metal, EffectValue);
        	break;
        case EStructureEffect::IncreasesAlienMaterialPerSecond:
			ResourcesToGeneratePerSecond.Add(EResourceType::AlienMaterial, EffectValue);
        	break;
        case EStructureEffect::IncreasesFoodPerSecond:
			ResourcesToGeneratePerSecond.Add(EResourceType::Food, EffectValue);
        	break;
		case EStructureEffect::IncreasesPowerPerSecond:
			ResourcesToGeneratePerSecond.Add(EResourceType::Power, EffectValue);
			break;			
        case EStructureEffect::IncreasesMetalStorage:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::Metal, EffectValue);
	        break;
        case EStructureEffect::IncreasesAlienMaterialStorage:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::AlienMaterial, EffectValue);
	        break;
		case EStructureEffect::IncreasesFoodStorage:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::Food, EffectValue);
	        break;			
		case EStructureEffect::IncreasesWorkerCapacity:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::Workers, EffectValue);
			break;
		case EStructureEffect::IncreasesScientistCapacity:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::Scientists, EffectValue);
			break;
		case EStructureEffect::IncreasesPowerCapacity:
			GetStrategyGameState()->IncreaseResourceStorage(EResourceType::Power, EffectValue);
			break;
		case EStructureEffect::ConsumesMetalPerSecond:
			ResourcesToConsumePerSecond.Add(EResourceType::Metal, EffectValue);
			break;
		case EStructureEffect::ConsumesAlienMaterialPerSecond:
			ResourcesToConsumePerSecond.Add(EResourceType::AlienMaterial, EffectValue);
			break;
		case EStructureEffect::ConsumesFoodPerSecond:
			ResourcesToConsumePerSecond.Add(EResourceType::Food, EffectValue);
			break;			
		case EStructureEffect::ConsumesPowerPerSecond:
			ResourcesToConsumePerSecond.Add(EResourceType::Power, EffectValue);
			break;
		}
	}

	if (!ResourcesToGeneratePerSecond.IsEmpty())
	{
		BeginGeneratingResources(ResourcesToGeneratePerSecond);
	}

	if (!ResourcesToConsumePerSecond.IsEmpty())
	{
		BeginConsumingResources(ResourcesToConsumePerSecond);
	}
}

void ABuildableStructure::BeginGeneratingResources(TMap<EResourceType, int32> ResourcesToGeneratePerSecond)
{
	FTimerDelegate ResourceGenerationDelegate;
	ResourceGenerationDelegate.BindUObject(this, &ABuildableStructure::GenerateResources, ResourcesToGeneratePerSecond);
	GetWorldTimerManager().SetTimer(ResourceGenerationTimer, ResourceGenerationDelegate, 1.0f, true);
}

void ABuildableStructure::GenerateResources(TMap<EResourceType, int32> ResourcesToGeneratePerSecond)
{
	for (auto Resource : ResourcesToGeneratePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->AddResources(ResourceType, ResourceAmount);
	}
}

void ABuildableStructure::BeginConsumingResources(TMap<EResourceType, int32> ResourcesToConsumePerSecond)
{
	FTimerDelegate ResourceConsumptionDelegate;
	ResourceConsumptionDelegate.BindUObject(this, &ABuildableStructure::ConsumeResources, ResourcesToConsumePerSecond);
	GetWorldTimerManager().SetTimer(ResourceConsumptionTimer, ResourceConsumptionDelegate, 1.0f, true);
}

void ABuildableStructure::ConsumeResources(TMap<EResourceType, int32> ResourcesToConsumePerSecond)
{
	for (auto Resource : ResourcesToConsumePerSecond)
	{
		EResourceType ResourceType = Resource.Key;
		int32 ResourceAmount = Resource.Value;
		
		GetStrategyGameState()->ConsumeResources(ResourceType, ResourceAmount);
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

