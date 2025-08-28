// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Components/TextRenderComponent.h"
#include "Interfaces/PowerInterface.h"
#include "Structure.generated.h"

UCLASS()
class STRATEGYGAME_API AStructure : public ABuildable, public IPowerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStructure();

protected:

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* StructureText;

	UPROPERTY() FTimerHandle ResourceGenerationTimer;
	UPROPERTY() FTimerHandle ResourceConsumptionTimer;
	UPROPERTY() FTimerHandle ResourceDrainingTimer;

	// ------ STRUCTURE EFFECTS ------

	// How much this structure increases population capacity;
	UPROPERTY(EditDefaultsOnly, Category="Structure Effects", meta=(ClampMin=0))
	int32 AdditionalPopulationCapacity = 0;

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

	// ------ POWER ------

	UPROPERTY() TArray<APowerLine*> ConnectedPowerLines;

	// ------ WORKERS ------

	UPROPERTY(EditAnywhere, Category="Workers")
	bool bAllowWorkerEmployment = false;
	
	UPROPERTY(EditAnywhere, Category="Workers")
	bool bAllowScientistEmployment = false;
	
	UPROPERTY() TMap<ECitizenType, int32> AssignedWorkers;

	UPROPERTY(EditAnywhere, Category="Workers", meta=(EditCondition="bAllowWorkerEmployment || bAllowScientistEmployment"))
	int32 MaxWorkerCapacity = 15;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:

	// ------ INTERFACE FUNCTIONS ------
	
	virtual bool Select_Implementation(ARTSCamera* SelectInstigator) override;
	
	virtual bool ConnectPower_Implementation(APowerLine* PowerLine) override;
	virtual bool DisconnectPower_Implementation(APowerLine* PowerLine) override;

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
	
	UFUNCTION(BlueprintCallable)
	void AssignWorkers(ECitizenType WorkerType, int32 Amount);
	UFUNCTION(BlueprintCallable)
	void AddMaxWorkers(ECitizenType WorkerType);

	UFUNCTION(BlueprintCallable)
	void RemoveWorkers(ECitizenType WorkerType, int32 Amount);
	UFUNCTION(BlueprintCallable)
	void RemoveAllWorkers(ECitizenType WorkerType);

	virtual void UpdateBuildMaterials() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ConsumesResourcesFromNearbyNode() { return bConsumesResourceFromNearbyNode; }

	virtual bool IsBuildingPermitted() override;
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="IsBuildingPermitted")
	bool BP_IsBuildingPermitted() { return IsBuildingPermitted(); }
	

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power")
	bool DoesRequiresPowerConnection() { return ResourcesToConsumePerSecond.Contains(EResourceType::Power) && ResourcesToConsumePerSecond.FindRef(EResourceType::Power) > 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power")
	bool DoesGeneratePower() { return ResourcesToGeneratePerSecond.Contains(EResourceType::Power) && ResourcesToGeneratePerSecond.FindRef(EResourceType::Power) > 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power")
	bool IsConnectedToPowerLines() { return !ConnectedPowerLines.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	bool DoesIncreasePopulationCapacity() { return AdditionalPopulationCapacity > 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetAdditionalPopulationCapacity() { return AdditionalPopulationCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool GetAllowWorkerEmployment() { return bAllowWorkerEmployment; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool GetAllowScientistEmployment() { return bAllowScientistEmployment; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetWorkerCount(ECitizenType WorkerType) { return AssignedWorkers.FindRef(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetMaxWorkerCapacity() { return MaxWorkerCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetTotalWorkers() { return GetWorkerCount(ECitizenType::Worker) + GetWorkerCount(ECitizenType::Scientist); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetAvailableWorkersSlots() { return GetMaxWorkerCapacity() - GetTotalWorkers(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool IsWorkerCapacityFull() { return GetAvailableWorkersSlots() <= 0; }

	// Returns a value between 0 and 1 based on how many workers are assigned to the structure.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	float GetWorkerEfficiency() { if (!bAllowWorkerEmployment && !bAllowScientistEmployment) return 1; return static_cast<float>(GetTotalWorkers()) / MaxWorkerCapacity; }
};
