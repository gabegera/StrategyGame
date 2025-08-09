// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameStates/StrategyGameState.h"
#include "Actors/ResourceNode.h"
#include "Interfaces/SelectionInterface.h"
#include "BuildableStructure.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class EStructureMode : uint8
{
	BeingCreated		UMETA(DisplayName="Being Created"),
	UnderConstruction	UMETA(DisplayName="Under Construction"),
	BeingMoved			UMETA(DisplayName="Being Moved"),
	Complete			UMETA(DisplayName="Complete"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStructureSelectedDelegate, ABuildableStructure*, NewSelectedStructure);

UCLASS()
class STRATEGYGAME_API ABuildableStructure : public AActor, public ISelectionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildableStructure();

protected:

	// ------ DELEGATES ------
	
	UPROPERTY() FOnStructureSelectedDelegate OnStructureSelectedDelegate;

	// ------ USE THE GETTER FUNCTIONS FOR ANY POINTERS HERE ------
	UPROPERTY() AStrategyGameState* StrategyGameState = nullptr;

	// ------ COMPONENTS ------
	
	UPROPERTY(EditAnywhere, Category="Structure Components")
	UStaticMeshComponent* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category="Structure Components")
	UBoxComponent* StructureBounds;

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* ForwardArrow;

	// ------ STRUCTURE INFO ------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Structure Info")
	FString DisplayName = "Unnamed Structure";

	// ------ VARIABLES & REFERENCES ------
	UPROPERTY() EStructureMode StructureMode = EStructureMode::BeingCreated;
	UPROPERTY() TArray<AActor*> OverlappingExclusionZones;
	UPROPERTY() TArray<AActor*> OverlappingResourceNodes;
	UPROPERTY() AResourceNode* TargetResourceNode;

	// ------ CONSTRUCTION ------

	FTimerHandle ConstructionTimer;

	// How long it takes for the structure to be built.
	UPROPERTY(EditDefaultsOnly, Category="Construction")
	float ConstructionTime = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category="Construction|Materials")
	UMaterialInstance* CanBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Construction|Materials")
	UMaterialInstance* CanNotBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Construction|Materials")
	UMaterialInstance* IsBuildingMaterial;

	// ------ RESOURCES ------

	UPROPERTY(EditDefaultsOnly, Category="Construction")
	TMap<EResourceType, int32> ConstructionCost;

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

	// ------ PROTECTED FUNCTIONS ------

	virtual void PostInitializeComponents() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	virtual bool Select_Implementation(ARTSCamera* SelectInstigator) override;

	UFUNCTION(BlueprintCallable)
	bool BeginConstruction();

	UFUNCTION(BlueprintCallable)
	void CancelConstruction();
	void ConsumeConstructionResources();
	void RefundConstructionMaterials();
	void CompleteConstruction();

	// Changes the mesh material depending on if the structure is being placed, is being constructed, or is unable to be built.
	UFUNCTION(BlueprintCallable)
	void UpdateBuildMaterials();

	UFUNCTION(BlueprintCallable)
	void ActivateStructureEffects();

	UFUNCTION(BlueprintCallable)
	AResourceNode* FindClosestResourceNode();

	UFUNCTION(BlueprintCallable)
	void BeginGeneratingResources();
	void GenerateResources();

	UFUNCTION(BlueprintCallable)
	void BeginConsumingResources();
	void ConsumeResources();
	
	UFUNCTION(BlueprintCallable)
	void BeginDrainingResourceFromNode();
	void DrainResourceFromNode();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetDisplayName() { return DisplayName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EStructureMode GetStructureMode() { return StructureMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBeingCreated() { return StructureMode == EStructureMode::BeingCreated; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnderConstruction() { return StructureMode == EStructureMode::UnderConstruction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsCompleted() { return StructureMode == EStructureMode::Complete; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOverlappingBuildExclusionZone() { return !OverlappingExclusionZones.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOverlappingResourceNode() { return !OverlappingResourceNodes.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetOverlappingBuildExclusionZones() { return OverlappingExclusionZones; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveEnoughResourcesToBuild();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ConsumesResourcesFromNearbyNode() { return bConsumesResourceFromNearbyNode; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, int32> GetConstructionResourceCost() { return ConstructionCost; }

};
