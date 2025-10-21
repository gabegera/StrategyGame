// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ResourceStorageComponent.h"


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

TArray<UResourceDataAsset*> UResourceStorageComponent::GetStorageResources() const
{
	TArray<UResourceDataAsset*> OutArray;
	ResourcesToIncreaseStorage.GetKeys(OutArray);
	return OutArray;
}
