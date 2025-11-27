// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StructureInterface.generated.h"

class UResourceDataAsset;
class ARTSCamera;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStructureInterface : public UInterface
{
	GENERATED_BODY()
};

class STRATEGYGAME_API IStructureInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual bool TrySelect(ARTSCamera* SelectInstigator);
	
	virtual bool TryRecycle(ARTSCamera* DestroyInstigator);

	virtual bool DoesIncreaseStorage();

	virtual bool DoesGenerateResources();

	virtual bool DoesHarvestResources();

	virtual bool DoesProvideHousing();

	virtual bool DoesRequireWorkers();
};
