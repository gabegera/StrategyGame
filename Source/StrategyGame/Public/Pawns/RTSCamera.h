// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Buildable.h"
#include "Camera/CameraComponent.h"
#include "Controllers/RTSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "RTSCamera.generated.h"

class AStrategyGameState;
class APlayerCharacter;

UENUM()
enum ERTSTool
{
	SelectTool		UMETA(DisplayName="Select Tool"),
	RecycleTool		UMETA(DisplayName="Destroy Tool"),
	MoveTool		UMETA(DisplayName="Move Tool"),
};

UCLASS()
class STRATEGYGAME_API ARTSCamera : public APawn, public IBuildingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCamera();

protected:

	UPROPERTY() FBuildableSelectedDelegate StructureSelectedDelegate;

	UPROPERTY() ARTSPlayerController* RTSPlayerController = nullptr;
	UPROPERTY() AStrategyGameState* StrategyGameState = nullptr;

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
	
	// The Structure that has been selected / clicked on or is being built / placed.
	UPROPERTY() ABuildable* SelectedBuildable;

	UPROPERTY(EditAnywhere, Category="Structure Building")
	int32 SnappingSize = 50;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStructureSelected(ABuildable* Selection);

	UFUNCTION()
	virtual void OnResourcesChanged();

	

public:
	
	void Move(FVector2D MoveInput);
	
	void RotateCamera(float Input);

	UFUNCTION(BlueprintCallable)
	void UpdateCameraPitch();
	
	void Zoom(float Input);
	void UpdateZoom();

	// Interacts with the target at the mouse cursor.
	void SelectTarget();
	
	void CancelAction();
	
	void EquipRecycleTool();
	
	void EquipMoveTool();
	
	void BuildStructure();

	UFUNCTION(BlueprintCallable)
	void EnableSelectTool();
	
	UFUNCTION(BlueprintCallable)
	void EnableDestroyTool();

	UFUNCTION(BlueprintCallable)
	void EnableMoveTool();
	
	UFUNCTION(BlueprintCallable)
	void ExitRTSMode();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult LineTraceToMousePos(ECollisionChannel CollisionChannel);

	UFUNCTION(BlueprintCallable)
	void SelectStructureBlueprint(TSubclassOf<ABuildable> NewBlueprint);

	UFUNCTION(BlueprintCallable)
	void MoveStructureToMousePos();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ABuildable* SetSelectedStructure(ABuildable* NewSelectedStructure) { return SelectedBuildable = NewSelectedStructure; }

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARTSPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABuildable* GetSelectedStructure() { return SelectedBuildable; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMinZoomHeight() { return ZoomDistanceMin; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxZoomHeight() { return ZoomDistanceMax; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetZoomDistanceAlpha() { return (SpringArm->TargetArmLength - ZoomDistanceMin) / (ZoomDistanceMax - ZoomDistanceMin); }

};
