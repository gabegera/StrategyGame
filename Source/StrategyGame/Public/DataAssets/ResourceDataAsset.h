// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ResourceDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class STRATEGYGAME_API UResourceDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintGetter=GetResourceName, Category="Resource")
	FText ResourceName;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetResourceIcon, Category="Resource")
	TSoftObjectPtr<UTexture2D> ResourceIcon;

public:

	UFUNCTION(BlueprintPure, Category="Resource")
	FText GetResourceName() const { return ResourceName; }

	UFUNCTION(BlueprintPure, Category="Resource")
	TSoftObjectPtr<UTexture2D> GetResourceIcon() const { return ResourceIcon; }
};
