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

void ASkyscraper::CompleteConstruction()
{
	Super::CompleteConstruction();

	BuildModules();
}

void ASkyscraper::BuildModules()
{
	verifyf(ModuleClass, TEXT("ASkyscraper::BuildModules ModuleClass is not set."))

	for (auto Module : Modules)
	{
		Module->Destroy();
	}
		
	Modules.SetNum(MaxSkyscraperSections);

	for (int32 i = 0; i < MaxSkyscraperSections; i++)
	{
		ASkyscraperModule* NewModule = GetWorld()->SpawnActor<ASkyscraperModule>(ModuleClass);
		
		if (i == MaxSkyscraperSections - 1) // If is the last module
		{
			NewModule->SwitchToTopMesh();
		}
		else
		{
			NewModule->SwitchToDefaultMesh();
		}
			
		Modules[i] = NewModule;

		if (i > 0) // If the module isn't at the bottom of the stack.
		{
			ASkyscraperModule* PrevModule = Modules[i-1];
			FVector PrevModuleBounds = PrevModule->GetStaticMeshComponent()->GetStaticMesh()->GetBounds().BoxExtent;
			NewModule->SetActorLocation(PrevModule->GetActorLocation() + FVector::UpVector * PrevModuleBounds.Z * 2);
		}
		else
		{
			FVector SkyscraperFoundationBounds = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
			NewModule->SetActorLocation(StaticMeshComponent->GetComponentLocation() + FVector::UpVector * SkyscraperFoundationBounds.Z * 2);
		}

		NewModule->AttachToComponent(StaticMeshComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ASkyscraper::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ASkyscraper::AddModule(TSubclassOf<ASkyscraperModule> ModuleToAdd)
{
	
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
	
	return Super::Select_Implementation(SelectInstigator);
}

// Called every frame
void ASkyscraper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

