// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameStates/StrategyGameState.h"
#include "Actors/CustomActor.h"
#include "ResourceNode.generated.h"

class ABuildable;

UCLASS()
class STRATEGYGAME_API AResourceNode : public ACustomActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResourceNode();

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY() ABuildable* AssignedExtractor;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	EResourceType ResourceType = EResourceType::Metal;

	UPROPERTY(EditDefaultsOnly, Category="Resources")
	int32 ResourceAmount = 500;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DrainResource(int32 DecreaseAmount);

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	void SetAssignedExtractor(ABuildable* NewExtractor);

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetResourceAmount() { return ResourceAmount; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	EResourceType GetResourceType() { return ResourceType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABuildable* GetAssignedExtractor() { return AssignedExtractor; }
};
