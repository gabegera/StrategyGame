// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Components/TextRenderComponent.h"
#include "DataTables/StructureData.h"
#include "Interfaces/PowerInterface.h"
#include "Structure.generated.h"

// Child of ABuildable that has the ability to change resource and population values.
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

	// ------ STRUCTURE DATA ------

	UPROPERTY(EditDefaultsOnly, Category="Structure Data")
	FDataTableRowHandle StructureDataTableRow;
	
	UPROPERTY() TMap<ECitizenType, int32> AssignedWorkers;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:

	// ------ INTERFACE FUNCTIONS ------
	
	virtual bool Select_Implementation(ARTSCamera* SelectInstigator) override;

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

	
	// ------ GETTERS ------

	virtual bool IsBuildingPermitted() override;
    UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="IsBuildingPermitted")
    bool BP_IsBuildingPermitted() { return IsBuildingPermitted(); }
	
	const FStructureData* GetStructureData();
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="GetStructureData")
	FStructureData BP_GetStructureData() { return *GetStructureData();}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetGeneratesResources() { return GetStructureData()->bGeneratesResources; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetConsumesResources() { return GetStructureData()->bConsumesResources; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetConsumesResourcesFromNearbyNode() { return GetStructureData()->bConsumesResourceFromNearbyNode; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIncreasesStorageCapacity() { return GetStructureData()->bIncreasesStorageCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, float> GetResourcesToGeneratePerSecond() { return GetStructureData()->ResourcesToGeneratePerSecond; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, float> GetResourcesToConsumePerSecond() { return GetStructureData()->ResourcesToConsumePerSecond; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, int32> GetResourcesToIncreaseStorage() { return GetStructureData()->ResourcesToIncreaseStorage; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	bool DoesIncreasePopulationCapacity() { return GetStructureData()->AdditionalPopulationCapacity > 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Population")
	int32 GetAdditionalPopulationCapacity() { return GetStructureData()->AdditionalPopulationCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool GetAllowWorkerEmployment() { return GetStructureData()->bAllowWorkerEmployment; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool GetAllowScientistEmployment() { return GetStructureData()->bAllowScientistEmployment; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetWorkerCount(ECitizenType WorkerType) { return AssignedWorkers.FindRef(WorkerType); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetMaxWorkerCapacity() { return GetStructureData()->MaxWorkerCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetTotalWorkers() { return GetWorkerCount(ECitizenType::Worker) + GetWorkerCount(ECitizenType::Scientist); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	int32 GetAvailableWorkersSlots() { return GetMaxWorkerCapacity() - GetTotalWorkers(); }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	bool IsWorkerCapacityFull() { return GetAvailableWorkersSlots() <= 0; }

	// Returns a value between 0 and 1 based on how many workers are assigned to the structure.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Workers")
	float GetWorkerEfficiency() { if (!GetAllowWorkerEmployment() && !GetAllowScientistEmployment()) return 1; return static_cast<float>(GetTotalWorkers()) / GetMaxWorkerCapacity(); }
};
