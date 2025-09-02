// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Building/Buildable.h"
#include "Camera/CameraComponent.h"
#include "Controllers/RTSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/StrategyGameModeBase.h"
#include "Interfaces/BuildingInterface.h"
#include "RTSCamera.generated.h"

class ARoad;
class AStrategyGameModeBase;
class AStrategyGameState;
class APlayerCharacter;

UENUM()
enum ERTSTool
{
	SelectTool			UMETA(DisplayName="Select Tool"),
	RecycleTool			UMETA(DisplayName="Destroy Tool"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBuildableSelectedDelegate, ABuildable*, SelectedBuildable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBuildableDeSelectedDelegate);

UCLASS()
class STRATEGYGAME_API ARTSCamera : public APawn, public IBuildingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCamera();

protected:

	UPROPERTY() ARTSPlayerController* RTSPlayerController = nullptr;
	UPROPERTY() AStrategyGameState* StrategyGameState = nullptr;
	UPROPERTY() AStrategyGameModeBase* StrategyGameMode = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere, Category="Components")
	UCameraComponent* Camera = nullptr;

	UPROPERTY(EditAnywhere, Category="Components")
	USpringArmComponent* SpringArm = nullptr;

	// ------ MOVEMENT ------
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float MoveSpeed = 60.0f;

	UPROPERTY() float ZoomDistanceTarget = 0.0f;

	// Controls how fast the target zoom distance is adjusted.
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float ZoomSpeed = 300.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float ZoomDistanceMax = 100000.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float ZoomDistanceMin = 30000.0f;

	// Exponent value for easing the zoom.
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float ZoomEasing = 2.0f;

	// How quickly the camera reaches its target zoom distance. Higher values are faster, smaller values are slower.
	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float ZoomEasingSpeed = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float CameraPitchAtMaxZoom = 75.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float CameraPitchAtMinZoom = 40.0f;

	UPROPERTY(EditDefaultsOnly, Category="Movement")
	float CameraRotationSpeed = 360.0f;

	// ------ TOOLS ------

	ERTSTool CurrentRTSTool = ERTSTool::SelectTool;

	// ------ STRUCTURE BUILDING ------
	
	// The buildable that has been selected to be constructed.
	UPROPERTY() ABuildable* BuildableBlueprint = nullptr;

	// The Structure that has been clicked on / selected.
	UPROPERTY() ABuildable* SelectedBuildable = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBuildableSelectedDelegate OnBuildableSelected;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FBuildableDeSelectedDelegate OnBuildableDeSelected;
	
	void Move(FVector2D MoveInput);
	
	void RotateCamera(float Input);

	UFUNCTION(BlueprintCallable)
	void UpdateCameraPitch();
	
	void Zoom(float Input);
	void UpdateZoom();

	// Interacts with the target at the mouse cursor.
	void SelectTarget();

	void DeselectTarget();
	
	void CancelAction();
	
	void PlaceBlueprint();

	void RotateBuilding();

	void EquipRecycleTool();
	
	UFUNCTION(BlueprintCallable)
	void ExitRTSMode();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult LineTraceToMousePos(ECollisionChannel CollisionChannel = ECC_Visibility);

	UFUNCTION(BlueprintCallable)
	void SelectBuildableBlueprint(TSubclassOf<ABuildable> NewBlueprint);

	UFUNCTION(BlueprintCallable)
	void MoveBlueprintToMousePos();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ABuildable* SetSelectedBuildable(ABuildable* NewSelectedBuildable) { return SelectedBuildable = NewSelectedBuildable; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector SnapVectorToGrid(FVector InputPos, int32 GridSize);

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARTSPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintGetter)
	AStrategyGameModeBase* GetStrategyGameMode();

	UFUNCTION(BlueprintGetter)
	int32 GetSnappingSize() { return GetStrategyGameMode()->GetSnappingSize(); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABuildable* GetSelectedBuildable() { return SelectedBuildable; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABuildable* GetBuildableBlueprint() { return BuildableBlueprint; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMinZoomHeight() { return ZoomDistanceMin; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxZoomHeight() { return ZoomDistanceMax; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetZoomDistanceAlpha() { return (SpringArm->TargetArmLength - ZoomDistanceMin) / (ZoomDistanceMax - ZoomDistanceMin); }

};
