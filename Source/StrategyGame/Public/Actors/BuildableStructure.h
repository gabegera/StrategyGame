// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GameStates/StrategyGameState.h"
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

UENUM(BlueprintType)
enum class EStructureEffect : uint8
{
	IncreasesMetalPerSecond				UMETA(DisplayName="Increases Metal Every Second"),
	IncreasesAlienMaterialPerSecond		UMETA(DisplayName="Increases Alien Material Every Second"),
	IncreasesFoodPerSecond				UMETA(DisplayName="Increases Food Every Second"),
	IncreasesPowerPerSecond				UMETA(DisplayName="Increases Power Every Second"),
	IncreasesMetalStorage		 		UMETA(DisplayName="Increases Metal Storage"),
	IncreasesAlienMaterialStorage 		UMETA(DisplayName="Increases Alien Material Storage"),
	IncreasesFoodStorage		 		UMETA(DisplayName="Increases Food Storage"),
	IncreasesPowerCapacity				UMETA(DisplayName="Increases Power Capacity"),
	IncreasesWorkerCapacity 			UMETA(DisplayName="Increases Worker Capacity"),
	IncreasesScientistCapacity 			UMETA(DisplayName="Increases Scientist Capacity"),
	ConsumesMetalPerSecond		 		UMETA(DisplayName="Consumes Metal Every Second"),
	ConsumesAlienMaterialPerSecond		UMETA(DisplayName="Consumes Alien Material Every Second"),
	ConsumesFoodPerSecond		 		UMETA(DisplayName="Consumes Food Every Second"),
	ConsumesPowerPerSecond		 		UMETA(DisplayName="Consumes Power Every Second"),
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
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EStructureEffect, int32> StructureEffects;

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

	UPROPERTY(EditDefaultsOnly)
	TMap<EResourceType, int32> ConstructionCost;

	UPROPERTY() FTimerHandle ResourceGenerationTimer;
	UPROPERTY() FTimerHandle ResourceConsumptionTimer;

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
	
	void CompleteConstruction();

	UFUNCTION(BlueprintCallable)
	void ActivateStructureEffects();

	UFUNCTION(BlueprintCallable)
	void BeginGeneratingResources(TMap<EResourceType, int32> ResourcesToGeneratePerSecond);
	void GenerateResources(TMap<EResourceType, int32> ResourcesToGeneratePerSecond);

	UFUNCTION(BlueprintCallable)
	void BeginConsumingResources(TMap<EResourceType, int32> ResourcesToConsumePerSecond);
	void ConsumeResources(TMap<EResourceType, int32> ResourcesToConsumePerSecond);
	
	
	
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
	TArray<AActor*> GetOverlappingBuildExclusionZones() { return OverlappingExclusionZones; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HaveEnoughResourcesToBuild();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TMap<EResourceType, int32> GetConstructionResourceCost() { return ConstructionCost; }

};
