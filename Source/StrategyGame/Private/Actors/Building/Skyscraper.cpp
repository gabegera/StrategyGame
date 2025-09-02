// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Skyscraper.h"

#include "Pawns/RTSCamera.h"


// Sets default values
ASkyscraper::ASkyscraper()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkyscraper::BeginPlay()
{
	Super::BeginPlay();
}

void ASkyscraper::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASkyscraper::AddModule(TSubclassOf<ASkyscraperModule> ModuleToAdd)
{
	if (Modules.Num() >= MaxSkyscraperModules) return;

	ASkyscraperModule* NewModule = GetWorld()->SpawnActor<ASkyscraperModule>(ModuleToAdd);
	Modules.AddUnique(NewModule);

	if (Modules.Last() == NewModule)
    {
    	NewModule->SwitchToTopMesh();
		for (auto Module : Modules)
		{
			if (Module != NewModule) Module->SwitchToDefaultMesh();
		}
    }
    else
    {
    	NewModule->SwitchToDefaultMesh();
    }

	if (Modules.Num() > 1)
	{
		ASkyscraperModule* PrevModule = Modules.Last(1);
		FVector PrevModuleBounds = PrevModule->GetStaticMeshComponent()->GetStaticMesh()->GetBounds().BoxExtent;
		NewModule->SetActorLocation(PrevModule->GetActorLocation() + FVector::UpVector * PrevModuleBounds.Z * 2);
	}
	else
	{
		FVector SkyscraperFoundationBounds = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
		NewModule->SetActorLocation(StaticMeshComponent->GetComponentLocation() + FVector::UpVector * SkyscraperFoundationBounds.Z * 2);
	}

	NewModule->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepWorldTransform);

	GetStrategyGameState()->OnSkyscraperModuleAdded.Broadcast(this, NewModule);
}

void ASkyscraper::Recycle()
{
	for (ASkyscraperModule* Module : Modules)
	{
		Module->Recycle();
	}
	
	Super::Recycle();
}

bool ASkyscraper::Select_Implementation(ARTSCamera* SelectInstigator)
{
	SelectInstigator->SetSelectedBuildable(this);
	
	return true;
}

// Called every frame
void ASkyscraper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

