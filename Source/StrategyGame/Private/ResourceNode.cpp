// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceNode.h"

#include "Components/LookAtCameraTextRenderComponent.h"


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
}

// Called when the game starts or when spawned
void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
}

float AResourceNode::TryDrainResource(const float DrainAmount)
{
	return DrainResource(DrainAmount);
}

UResourceDataAsset* AResourceNode::TryGetResourceType()
{
	return GetResourceType();
}

float AResourceNode::TryGetResourceAmount()
{
	return GetResourceAmount();
}

float AResourceNode::DrainResource(const float DrainAmount)
{
	const float AmountBeforeDrain = ResourceAmount;
	ResourceAmount = FMath::Clamp(ResourceAmount - DrainAmount, 0.0f, ResourceAmount);
	const float AmountDrained = AmountBeforeDrain - ResourceAmount;

	if (ResourceAmount <= 0)
	{
		Destroy();
	}

	return AmountDrained;
}

float AResourceNode::GetResourceAmount() const
{
	return ResourceAmount;
}

UResourceDataAsset* AResourceNode::GetResourceType() const
{
	return ResourceType;
}

