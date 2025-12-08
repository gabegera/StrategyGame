// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceNode.h"

#include "Components/LookAtCameraTextRenderComponent.h"
#include "DataAssets/ResourceDataAsset.h"


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

	// ResourceText = CreateDefaultSubobject<ULookAtCameraTextRenderComponent>("Text");
	// ResourceText->SetupAttachment(StaticMesh);
	// ResourceText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	// ResourceText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextBottom;
	// ResourceText->WorldSize = 156.0f;
}

// Called when the game starts or when spawned
void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceNode::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// const FVector ResourceTextLocation = FVector::UpVector * (StaticMesh->Bounds.BoxExtent.Z * 2.0f) + 10.0f;
	// ResourceText->SetRelativeLocation(ResourceTextLocation);
	// UpdateResourceText();
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

// void AResourceNode::UpdateResourceText() const
// {
// 	if (ResourceType)
// 	{
// 		FString ResourceString = ResourceType->GetResourceName().ToString() + ": " + FString::SanitizeFloat(ResourceAmount);
// 		ResourceText->Text = FText::FromString(ResourceString);
// 	}
// }

float AResourceNode::DrainResource(const float DrainAmount)
{
	const float AmountBeforeDrain = ResourceAmount;
	ResourceAmount = FMath::Clamp(ResourceAmount - DrainAmount, 0.0f, ResourceAmount);
	const float AmountDrained = AmountBeforeDrain - ResourceAmount;

	if (ResourceAmount <= 0)
	{
		Destroy();
	}

	// UpdateResourceText();
	return AmountDrained;
}

float AResourceNode::GetResourceAmount() const
{
	return ResourceAmount;
}

float AResourceNode::SetResourceAmount(float InAmount)
{
	return ResourceAmount = InAmount;
}

UResourceDataAsset* AResourceNode::GetResourceType() const
{
	return ResourceType;
}

