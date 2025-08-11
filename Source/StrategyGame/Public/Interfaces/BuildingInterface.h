// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuildingInterface.generated.h"

class ARTSCamera;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBuildingInterface : public UInterface
{
	GENERATED_BODY()
};


class STRATEGYGAME_API IBuildingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Select(ARTSCamera* SelectInstigator);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Recycle(ARTSCamera* DestroyInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool Move(ARTSCamera* DestroyInstigator);
};
