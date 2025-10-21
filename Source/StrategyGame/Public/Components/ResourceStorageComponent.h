// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ResourceStorageComponent.generated.h"

class UResourceDataAsset;

UCLASS(ClassGroup=StructureComponents)
class STRATEGYGAME_API UResourceStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UResourceStorageComponent();

protected:

	UPROPERTY(EditAnywhere, Category="Resource Storage")
	TMap<UResourceDataAsset*, int32> ResourcesToIncreaseStorage;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure, Category="Resource Storage")
	TArray<UResourceDataAsset*> GetStorageResources() const;

	UFUNCTION(BlueprintPure, Category="Resource Storage")
	int32 GetResourceStorageAmount(const UResourceDataAsset* InResourceType) const { return ResourcesToIncreaseStorage.FindRef(InResourceType); }
	
};
