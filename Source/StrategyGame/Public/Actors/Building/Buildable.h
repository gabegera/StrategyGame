// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/CustomActor.h"
#include "Components/BoxComponent.h"
#include "GameStates/StrategyGameState.h"
#include "Actors/ResourceNode.h"
#include "Interfaces/BuildingInterface.h"
#include "Buildable.generated.h"

class UArrowComponent;

UENUM(BlueprintType)
enum class EBuildableMode : uint8
{
	BeingCreated			UMETA(DisplayName="Being Created"),
	UnderConstruction		UMETA(DisplayName="Under Construction"),
	ConstructionComplete	UMETA(DisplayName="Construction Complete"),
};

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMesh = nullptr;

	// A Mesh to show during building construction which direction is forward.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* ForwardIdentifierMesh = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UBoxComponent* BuildingBounds;

	// ------ STRUCTURE INFO ------

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Buildable")
	FString DisplayName = "Unnamed Buildable";

	// ------ VARIABLES & REFERENCES ------
	UPROPERTY() EBuildableMode StructureMode = EBuildableMode::BeingCreated;
	UPROPERTY() TArray<AActor*> OverlappingExclusionZones;
	UPROPERTY() TArray<AActor*> OverlappingResourceNodes;
	UPROPERTY() TArray<AActor*> OverlappingRoads;
	UPROPERTY() AResourceNode* TargetResourceNode;

	// ------ CONSTRUCTION ------

	FTimerHandle ConstructionTimer;

	// How long it takes for the structure to be built.
	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction")
	float ConstructionTime = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Buildable|Construction")
	TMap<EResourceType, int32> ConstructionCost;

	// The Offset to add to the snapping grid.
	UPROPERTY() FIntVector2 SnappingOffset = FIntVector2(0, 0);

	UPROPERTY() UMaterialInterface* StructureMaterial;	

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

	UFUNCTION() virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

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
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetDisplayName() { return DisplayName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetStaticMesh() { return StaticMesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EBuildableMode GetStructureMode() { return StructureMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntVector2 GetSnappingOffset() { return SnappingOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBeingCreated() { return StructureMode == EBuildableMode::BeingCreated; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnderConstruction() { return StructureMode == EBuildableMode::UnderConstruction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsConstructionComplete() { return StructureMode == EBuildableMode::ConstructionComplete; }

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
