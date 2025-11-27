// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Interfaces/ResourcesInterface.h"
#include "ResourceNode.generated.h"

class ULookAtCameraTextRenderComponent;
class AStructure;
class UResourceDataAsset;

UCLASS()
class STRATEGYGAME_API AResourceNode : public ACustomActor, public IResourcesInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceNode();

protected:

	UPROPERTY() USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	UResourceDataAsset* ResourceType;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	float ResourceAmount = 500.0f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TryDrainResource(float DrainAmount) override;

	virtual UResourceDataAsset* TryGetResourceType() override;

	virtual float TryGetResourceAmount() override;

public:

	UFUNCTION(BlueprintCallable)
	float DrainResource(const float DrainAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetResourceAmount() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UResourceDataAsset* GetResourceType() const;
};
