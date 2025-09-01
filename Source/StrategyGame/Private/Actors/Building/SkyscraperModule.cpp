// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/SkyscraperModule.h"

#include "Actors/Building/Structure.h"
#include "Pawns/RTSCamera.h"


// Sets default values
ASkyscraperModule::ASkyscraperModule()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASkyscraperModule::BeginPlay()
{
	Super::BeginPlay();

	SwitchToDefaultMesh();
}

void ASkyscraperModule::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SwitchToDefaultMesh();
}

bool ASkyscraperModule::Select_Implementation(ARTSCamera* SelectInstigator)
{
	Execute_Select(GetAttachParentActor(), SelectInstigator);

	return true;
}

bool ASkyscraperModule::Recycle_Implementation(ARTSCamera* DestroyInstigator)
{
	Execute_Recycle(GetAttachParentActor(), DestroyInstigator);
	
	return true;
}

void ASkyscraperModule::SwitchToTopMesh()
{
	if (StaticMeshComponent->GetStaticMesh() != TopMesh)
	{
		StaticMeshComponent->SetStaticMesh(TopMesh.LoadSynchronous());
	}
}

void ASkyscraperModule::SwitchToDefaultMesh()
{
	if (StaticMeshComponent->GetStaticMesh() != DefaultMesh)
	{
		StaticMeshComponent->SetStaticMesh(DefaultMesh.LoadSynchronous());
	}
}

// Called every frame
void ASkyscraperModule::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

