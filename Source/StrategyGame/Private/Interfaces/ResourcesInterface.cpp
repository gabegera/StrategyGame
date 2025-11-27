// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ResourcesInterface.h"


float IResourcesInterface::TryDrainResource(float DrainAmount)
{
	return 0.0f;
}

// Add default functionality here for any IResourcesInterface functions that are not pure virtual.
UResourceDataAsset* IResourcesInterface::TryGetResourceType()
{
	return nullptr;
}

float IResourcesInterface::TryGetResourceAmount()
{
	return 0.0f;
}
