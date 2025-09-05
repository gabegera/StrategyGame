// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Components/BoxComponent.h"
#include "Game/StrategyGameState.h"
#include "ResourceNode.h"
#include "Interfaces/BuildingInterface.h"
#include "Buildable.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class EBuildableState : uint8
{
	BeingCreated			UMETA(DisplayName="Being Created"),
	UnderConstruction		UMETA(DisplayName="Under Construction"),
	ConstructionComplete	UMETA(DisplayName="Construction Complete"),
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBuildableStateChangedDelegate, ABuildable*, NewBuildable, EBuildableState, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStructurePlacedDelegate, ABuildable*, NewBuildable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildableRecycledDelegate, ABuildable*, NewBuildable);


// The base class for any actor that can be built in RTS Mode.
UCLASS()
class STRATEGYGAME_API ABuildable : public ACustomActor, public IBuildingInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildable();

protected:

	// ------ COMPONENTS ------

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere, BlueprintGetter=GetStaticMeshComponent, Category="Components")
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* BuildingBounds;

	// ------ STRUCTURE INFO ------

	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetDisplayName, Category="Buildable")
	FString DisplayName = "Unnamed Buildable";

	// ------ VARIABLES & REFERENCES ------

	// Use the setter function if changing this variable, that way the delegate gets broadcast.
	UPROPERTY(BlueprintGetter=GetBuildableState)
	EBuildableState BuildableState = EBuildableState::ConstructionComplete;
	
	UPROPERTY() TArray<AActor*> OverlappingExclusionZones;
	UPROPERTY() TArray<AActor*> OverlappingResourceNodes;
	UPROPERTY() TArray<AActor*> OverlappingRoads;
	UPROPERTY() AResourceNode* TargetResourceNode;

	// ------ CONSTRUCTION ------

	UPROPERTY()
	FTimerHandle ConstructionTimer;

	// How long it takes for the structure to be built.
	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction")
	float TimeToCompleteConstruction = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction")
	TMap<EResourceType, int32> ConstructionCost;

	// The Offset to add to the snapping grid.
	UPROPERTY(BlueprintGetter=GetSnappingOffset)
	FIntVector2 SnappingOffset = FIntVector2(0, 0);

	UPROPERTY()
	UMaterialInterface* DefaultMaterial;	

	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction|Materials")
	UMaterialInstance* CanBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction|Materials")
	UMaterialInstance* CanNotBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction|Materials")
	UMaterialInstance* IsBuildingMaterial;
	

	// ------ PROTECTED FUNCTIONS ------
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void BeginDestroy() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FBuildableStateChangedDelegate BuildableStateChangedDelegate;

	UFUNCTION()
	void OnBuildableStateChanged(ABuildable* Buildable, EBuildableState NewBuildableState);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnBuildableStateChanged")
	void BP_OnBuildableStateChanged(ABuildable* Buildable, EBuildableState NewBuildableState);

	UFUNCTION()
	void OnTimeScaleChanged(const ETimeScale NewTimeScale);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnTimeScaleChanged")
	void BP_OnTimeScaleChanged(const ETimeScale NewTimeScale);

	// ------ INTERFACE FUNCTIONS ------

	virtual bool GetIsConstructionComplete_Implementation() override;
	virtual bool Recycle_Implementation(ARTSCamera* RecycleInstigator) override;

	UFUNCTION(BlueprintCallable)
	virtual void MoveBuilding(FVector NewLocation);
	
	UFUNCTION(BlueprintCallable)
	virtual void PlaceBuilding();
	
	// Function to be called when the building is placed.
	UFUNCTION(BlueprintCallable)
	virtual void BeginConstruction();
	
	// If the structure is being built, cancels it and gets the materials back.
	UFUNCTION(BlueprintCallable)
	virtual void CancelConstruction();
	void ConsumeConstructionResources();
	void RefundConstructionMaterials();
	virtual void CompleteConstruction();

	UFUNCTION(BlueprintCallable, DisplayName="Recycle")
	void BP_Recycle() { Recycle(); }
	
	// Begins recycling the structure to destroy it and get its materials back.
	virtual void Recycle();

	// Changes the mesh material depending on if the structure is being placed, is being constructed, or is unable to be built.
	virtual void UpdateBuildMaterials();

	// ------ GETTERS ------

	UFUNCTION(BlueprintGetter)
	FString GetDisplayName() { return DisplayName; }

	UFUNCTION(BlueprintGetter)
	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }

	UFUNCTION(BlueprintGetter)
	EBuildableState GetBuildableState() { return BuildableState; }

	UFUNCTION(BlueprintCallable)
	EBuildableState SetBuildableState(EBuildableState NewMode) { BuildableStateChangedDelegate.Broadcast(this, NewMode); return BuildableState = NewMode; }

	UFUNCTION(BlueprintGetter)
	FIntVector2 GetSnappingOffset() { return SnappingOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBeingCreated() { return BuildableState == EBuildableState::BeingCreated; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnderConstruction() { return BuildableState == EBuildableState::UnderConstruction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsConstructionComplete() { return BuildableState == EBuildableState::ConstructionComplete; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOverlappingBuildExclusionZone() { return !OverlappingExclusionZones.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOverlappingResourceNode() { return !OverlappingResourceNodes.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsConnectedToRoad() { return !OverlappingRoads.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsBuildingPermitted();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetOverlappingBuildExclusionZones() { return OverlappingExclusionZones; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveEnoughResourcesToBuild();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, int32> GetConstructionResourceCost() { return ConstructionCost; }

};
