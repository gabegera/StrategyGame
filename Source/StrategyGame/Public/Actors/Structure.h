// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Structure.generated.h"

UCLASS()
class STRATEGYGAME_API AStructure : public ABuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStructure();

protected:

	UPROPERTY() FTimerHandle ResourceGenerationTimer;
	UPROPERTY() FTimerHandle ResourceConsumptionTimer;
	UPROPERTY() FTimerHandle ResourceDrainingTimer;

	// ------ STRUCTURE EFFECTS ------

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects")
	bool bGeneratesResources = false;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects|Resource Generation", meta=(EditCondition="bGeneratesResources", EditConditionHides))
	TMap<EResourceType, float> ResourcesToGeneratePerSecond;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects")
	bool bConsumesResources = false;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects|Resource Consumption", meta=(EditCondition="bConsumesResources", EditConditionHides))
	bool bConsumesResourceFromNearbyNode = false;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects|Resource Consumption", meta=(EditCondition="bConsumesResources", EditConditionHides))
	TMap<EResourceType, float> ResourcesToConsumePerSecond;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects")
	bool bIncreasesStorageCapacity = false;

	UPROPERTY(EditDefaultsOnly, Category="Structure Effects|Resource Storage", meta=(EditCondition="bIncreasesStorageCapacity", EditConditionHides))
	TMap<EResourceType, int32> ResourcesToIncreaseStorage;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:

	UFUNCTION(BlueprintCallable)
	void ActivateStructureEffects();

	UFUNCTION(BlueprintCallable)
	AResourceNode* FindClosestResourceNode();

	UFUNCTION(BlueprintCallable, DisplayName="BeginConstruction")
	void BP_BeginConstruction() { BeginConstruction(); }
	
	virtual void BeginConstruction() override;
	virtual void CompleteConstruction() override;
	
	virtual void Recycle() override;

	// If the structure increases storage capacity, this function will revert that.
	// Used for when the structure is destroyed.
	void RevertStorageCapacity();

	UFUNCTION(BlueprintCallable)
	void BeginGeneratingResources();
	void GenerateResources();

	UFUNCTION(BlueprintCallable)
	void BeginConsumingResources();
	void ConsumeResources();
	
	UFUNCTION(BlueprintCallable)
	void BeginDrainingResourceFromNode();
	void DrainResourceFromNode();

	virtual void UpdateBuildMaterials() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ConsumesResourcesFromNearbyNode() { return bConsumesResourceFromNearbyNode; }

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="IsBuildingPermitted")
	bool BP_IsBuildingPermitted() { return IsBuildingPermitted(); }
	
	virtual bool IsBuildingPermitted() override;
};
