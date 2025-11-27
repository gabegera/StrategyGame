// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourceStorageComponent.h"

#include "Building/Structure.h"
#include "Game/ResourcesSubsystem.h"


// Sets default values for this component's properties
UResourceStorageComponent::UResourceStorageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UResourceStorageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UResourceStorageComponent::OnStructureBuilt(AStructure* BuiltStructure)
{
	Super::OnStructureBuilt(BuiltStructure);

	// if (BuiltStructure == GetOwner())
	// {
	// 	UResourcesSubsystem* ResourcesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UResourcesSubsystem>();
	//  for (TPair<UResourceDataAsset*, int32> ResourceStorage : ResourcesToIncreaseStorage)
	//  {
	//  	ResourcesSubsystem->IncreaseResourceStorage(ResourceStorage.Key, ResourceStorage.Value);
	//  }
	// }
}

void UResourceStorageComponent::OnStructureDestroyed(AStructure* DestroyedStructure)
{
	Super::OnStructureDestroyed(DestroyedStructure);

	// if (DestroyedStructure == GetOwner())
	// {
	// 	UResourcesSubsystem* ResourcesSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UResourcesSubsystem>();
	// 	for (TPair<UResourceDataAsset*, int32> ResourceStorage : ResourcesToIncreaseStorage)
	// 	{
	// 		ResourcesSubsystem->DecreaseResourceStorage(ResourceStorage.Key, ResourceStorage.Value);
	// 	}
	// }
}

TMap<UResourceDataAsset*, int32>& UResourceStorageComponent::GetResourcesToIncreaseStorage()
{
	return ResourcesToIncreaseStorage;
}

TArray<UResourceDataAsset*> UResourceStorageComponent::GetStorageResources() const
{
	TArray<UResourceDataAsset*> OutArray;
	ResourcesToIncreaseStorage.GetKeys(OutArray);
	return OutArray;
}
