// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Structure.h"
#include "Camera/CameraComponent.h"
#include "Player/RTSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/StructureInterface.h"
#include "RTSCamera.generated.h"

class ACityDefenseGameMode;
class AStrategyGameState;
class APlayerCharacter;

UENUM()
enum ERTSTool
{
	SelectTool			UMETA(DisplayName="Select Tool"),
	RecycleTool			UMETA(DisplayName="Destroy Tool"),
};

UCLASS()
class STRATEGYGAME_API ARTSCamera : public APawn, public IStructureInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCamera();

protected:

	UPROPERTY() ARTSPlayerController* RTSPlayerController = nullptr;

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

	ERTSTool CurrentRTSTool = SelectTool;

	// ------ STRUCTURE BUILDING ------
	
	UPROPERTY()
	AStructure* SelectedStructure = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStructureSelected(AStructure* InSelectedStructure);

	UFUNCTION()
	virtual void OnStructureDeSelected();

public:
	
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
	
	void PlaceStructure();

	void RotateStructure();

	void EquipRecycleTool();
	
	UFUNCTION(BlueprintCallable)
	void ExitRTSMode();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult LineTraceToMousePos(ECollisionChannel CollisionChannel = ECC_Visibility);

	UFUNCTION(BlueprintCallable)
	void SelectStructure(TSubclassOf<AStructure> NewStructure);

	UFUNCTION(BlueprintCallable)
	void MoveStructureToMousePos();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	AStructure* SetSelectedStructure(AStructure* NewSelectedStructure) { return SelectedStructure = NewSelectedStructure; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector SnapVectorToGrid(FVector InputPos, int32 GridSize);

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARTSPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStructure* GetSelectedStructure() { return SelectedStructure; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMinZoomHeight() { return ZoomDistanceMin; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxZoomHeight() { return ZoomDistanceMax; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetZoomDistanceAlpha() { return (SpringArm->TargetArmLength - ZoomDistanceMin) / (ZoomDistanceMax - ZoomDistanceMin); }

};
