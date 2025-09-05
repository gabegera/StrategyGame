// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceNode.h"


// Sets default values
AResourceNode::AResourceNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(SceneComponent);
	StaticMesh->SetCollisionProfileName("SelectableObject");
	StaticMesh->SetGenerateOverlapEvents(true);

	Sphere = CreateDefaultSubobject<USphereComponent>("Trigger");
	Sphere->SetupAttachment(StaticMesh);
	Sphere->SetCollisionProfileName("Trigger");
	Sphere->SetSphereRadius(2000.0f);
}

// Called when the game starts or when spawned
void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AResourceNode::DrainResource(int32 DecreaseAmount)
{
	ResourceAmount = FMath::Clamp(ResourceAmount - DecreaseAmount, 0, ResourceAmount);

	if (ResourceAmount <= 0)
	{
		Destroy();
	}
}

void AResourceNode::SetAssignedExtractor(ABuildable* NewExtractor)
{
	if (GetAssignedExtractor())
	{
		GEngine->AddOnScreenDebugMessage(960, 3.0f, FColor::Red, "Cannot assign new extractor, there is already one assigned.");
	}
	else AssignedExtractor = NewExtractor;
}

