// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Components/BoxComponent.h"
#include "Components/ActorComponent.h"
#include "ResourceNode.h"
#include "Interfaces/StructureInterface.h"
#include "Structure.generated.h"

class ULookAtCameraTextRenderComponent;
class UArrowComponent;
enum class EUpdateTransformFlags : int32;

UENUM(BlueprintType)
enum class EStructureState : uint8
{
	BeingPlaced				UMETA(DisplayName="Being Placed"),
	UnderConstruction		UMETA(DisplayName="Under Construction"),
	ConstructionComplete	UMETA(DisplayName="Construction Complete"),
};

UENUM(BlueprintType, DisplayName="Structure Category")
enum class EStructureCategory : uint8
{
	Miscellaneous			= 0		UMETA(DisplayName="Miscellaneous"),
	Housing					= 1		UMETA(DisplayName="Housing"),
	ResourceGeneration		= 2		UMETA(DisplayName="Resource Generation"),
	Storage					= 3		UMETA(DisplayName="Storage"),
	Defenses				= 4		UMETA(DisplayName="Defenses"),
	Power					= 5		UMETA(DisplayName="Power"),
};

// The base class for any actor that can be built in RTS Mode.
UCLASS(Abstract)
class STRATEGYGAME_API AStructure : public ACustomActor, public IStructureInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStructure();

protected:

	// ------ COMPONENTS ------

	UPROPERTY()
	USceneComponent* SceneComponent;
	
	UPROPERTY(BlueprintGetter=GetStaticMeshComponent, Category="Structure|Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, Category="Structure|Components")
	UBoxComponent* BuildingBounds;

	UPROPERTY(EditAnywhere)
	UArrowComponent* StructureEntranceArrow;

	UPROPERTY(EditAnywhere)
	ULookAtCameraTextRenderComponent* LookAtCameraTextRenderComponent;

	// ------ Structure INFO ------

	UPROPERTY(EditAnywhere, BlueprintGetter=GetStructureName, Category="Structure")
	FText StructureName;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetStructureCategory, Category="Structure")
	EStructureCategory StructureCategory = EStructureCategory::Miscellaneous;

	UPROPERTY(EditAnywhere, Category="Structure")
	TSet<TSoftObjectPtr<UStaticMesh>> MeshVariety;

	// Use the setter function if changing this variable, that way the delegate gets broadcast.
	UPROPERTY()
	EStructureState StructureState = EStructureState::ConstructionComplete;
	
	UPROPERTY()
	TArray<AActor*> OverlappingExclusionZones;

	// ------ CONSTRUCTION ------

	UPROPERTY()
	FTimerHandle ConstructionTimer;

	// How long it takes for the structure to be built.
	UPROPERTY(EditDefaultsOnly, Category="Structure|Construction")
	float TimeToCompleteConstruction = 3.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Structure|Construction")
	TMap<UResourceDataAsset*, int32> ConstructionCost;

	// The Offset to add to the snapping grid.
	UPROPERTY()
	FIntVector2 SnappingOffset = FIntVector2(0, 0);

	UPROPERTY()
	UMaterialInterface* DefaultMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Structure|Construction|Materials")
	UMaterialInstance* CanBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Structure|Construction|Materials")
	UMaterialInstance* CanNotBuildMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Structure|Construction|Materials")
	UMaterialInstance* IsBuildingMaterial;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void ConsumeConstructionResources();
	
	void RefundConstructionMaterials();
	
	virtual void CompleteConstruction();
	
	virtual void DestroyStructure();

	// Changes the mesh material depending on if the structure is being placed, is being constructed, or is unable to be built.
	virtual void UpdateBuildMaterials();

	// Randomly picks a mesh from the MeshVariety set and loads it asynchronously before setting it as the current mesh.
	void LoadRandomMesh() const;

	void OnMeshLoaded(const TSoftObjectPtr<UStaticMesh> LoadedMesh) const;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	virtual void OnStructureStateChanged(AStructure* Structure, EStructureState NewStructureState);

public:

	virtual bool TrySelect(ARTSCamera* SelectInstigator) override;

	virtual bool TryRecycle(ARTSCamera* DestroyInstigator) override;

	virtual bool DoesIncreaseStorage() override;

	virtual bool DoesGenerateResources() override;

	virtual bool DoesHarvestResources() override;

	virtual bool DoesProvideHousing() override;

	virtual bool DoesRequireWorkers() override;

	UFUNCTION(BlueprintCallable)
	virtual void MoveStructure(FVector NewLocation);
	
	UFUNCTION(BlueprintCallable)
	virtual void PlaceBuilding();
	
	// Function to be called when the building is placed.
	UFUNCTION(BlueprintCallable)
	virtual void BeginConstruction();
	
	// If the structure is being built, cancels it and gets the materials back.
	UFUNCTION(BlueprintCallable)
	virtual void CancelConstruction();
	
	// Begins recycling the structure to destroy it and get its materials back.
	UFUNCTION(BlueprintCallable)
	virtual void Recycle();

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Structure")
	FText GetStructureName() { return StructureName; }

	UFUNCTION(BlueprintPure, Category="Structure")
	EStructureCategory GetStructureCategory() const { return StructureCategory; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetStaticMeshComponent() { return StaticMeshComponent; }

	UFUNCTION(BlueprintPure, Category="Structure")
	FVector GetEntranceLocation() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EStructureState GetStructureState() const;

	UFUNCTION(BlueprintCallable)
	EStructureState SetStructureState(EStructureState NewMode);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FIntVector2 GetSnappingOffset() { return SnappingOffset; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsBeingPlaced() const { return StructureState == EStructureState::BeingPlaced; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsUnderConstruction() const { return StructureState == EStructureState::UnderConstruction; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsConstructionComplete() const { return StructureState == EStructureState::ConstructionComplete; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOverlappingBuildExclusionZone() { return !OverlappingExclusionZones.IsEmpty(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsBuildingPermitted();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetOverlappingBuildExclusionZones() { return OverlappingExclusionZones; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveEnoughResourcesToBuild();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<UResourceDataAsset*, int32> GetConstructionResourceCost() { return ConstructionCost; }

};
