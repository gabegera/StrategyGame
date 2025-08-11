// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ResourceNode.h"


// Sets default values
AResourceNode::AResourceNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("NoCollision");
	StaticMesh->SetGenerateOverlapEvents(false);

	SphereTrigger = CreateDefaultSubobject<USphereComponent>("Sphere Trigger");
	SphereTrigger->SetupAttachment(StaticMesh);
	SphereTrigger->SetCollisionProfileName("Trigger");
	SphereTrigger->SetGenerateOverlapEvents(true);
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

