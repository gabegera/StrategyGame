// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/Road.h"


// Sets default values
ARoad::ARoad()
{
	DisplayName = "Road";

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>("Road Spline Mesh");
	SplineMesh->SetupAttachment(SceneComponent);
	SplineMesh->SetHiddenInGame(true);

	bRequiresConnectionToRoad = false;
}

// Called when the game starts or when spawned
void ARoad::BeginPlay()
{
	Super::BeginPlay();
}

void ARoad::MoveBuilding(FVector NewLocation)
{
	if (RoadStartPos != FVector::ZeroVector)
	{
		RoadEndPos = NewLocation;
		
		SplineMesh->SetStartPosition(RoadStartPos);
		SplineMesh->SetEndPosition(RoadEndPos);
	}
	else
	{
		SetActorLocation(NewLocation);
	}
}

void ARoad::PlaceBuilding()
{
	if (RoadStartPos == FVector::ZeroVector)
	{
		RoadStartPos = GetActorLocation();
		StaticMesh->SetHiddenInGame(true);
		SplineMesh->SetHiddenInGame(false);
	}
	else if (RoadEndPos != FVector::ZeroVector)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Template = this;
		ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(GetClass(), GetActorTransform(), SpawnParameters);
		NewRoad->BeginConstruction();
	}
}

void ARoad::UpdateBuildMaterials()
{
	
}

// Called every frame
void ARoad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsBeingCreated() && RoadStartPos != FVector::ZeroVector)
	{
		RoadEndPos = GetActorLocation();
	}
}

