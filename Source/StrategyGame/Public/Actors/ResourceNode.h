// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameStates/StrategyGameState.h"
#include "GameFramework/Actor.h"
#include "ResourceNode.generated.h"

UCLASS()
class STRATEGYGAME_API AResourceNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceNode();

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	TMap<EResourceType, int32> ResourcesInNode;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, int32> GetResourcesInNode() { return ResourcesInNode; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool DoesNodeContainResource(EResourceType ResourceToCheck) { return ResourcesInNode.Contains(ResourceToCheck); }
	
};
