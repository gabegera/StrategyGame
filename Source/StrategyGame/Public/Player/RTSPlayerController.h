// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.generated.h"

class APlayerCharacter;
class ARTSCamera;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContollerModeChangedDelegate);

UENUM(BlueprintType)
enum class EControllerMode : uint8
{
	FirstPerson UMETA(DisplayName="First Person Mode"),
	RTS			UMETA(DisplayName="RTS Mode"),
	Turret		UMETA(DisplayName="Turret Mode"),
};

UCLASS()
class STRATEGYGAME_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintAssignable)
	FOnContollerModeChangedDelegate OnControllerModeChangedDelegate;

	// WARNING: Do not call this directly, Call GetPlayerCharacter();
	UPROPERTY() APlayerCharacter* PlayerCharacter = nullptr;

	// WARNING: Do not call this directly, Call GetRTSCamera();
	UPROPERTY() ARTSCamera* RTSCamera = nullptr;

	EControllerMode ControllerMode = EControllerMode::FirstPerson;

	bool IsHoveringOverUI = false;

	// ------ INPUT ------
	
	// Default Mapping Context used for Enhanced Input.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> PlayerInputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Move;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Look;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Sprint;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Interact;
	
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_Move;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_PanCamera;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_RotateCamera;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_Mouse_RotateCamera;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_MouseInput;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_Zoom;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_Select;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_Cancel;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_RotateBuilding;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_EquipRecycleTool;	
	
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Look;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Fire;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Aim;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Reload;

	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_ReturnToFirstPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float FP_MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float Turret_MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RTS_PanSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RTS_RotateSensitivity = 1.0f;

	UPROPERTY() FVector2D MovementInput = FVector2D::ZeroVector;
	UPROPERTY() bool bIsPanBeingHeld = false;
	UPROPERTY() bool bIsMouseRotateBeingHeld = false;

	UFUNCTION(BlueprintCallable, Category="Input")
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

public:

	// ------ FIRST PERSON FUNCTIONS ------
	
	void FP_Move(const FInputActionInstance& Instance);
	
	void FP_Look(const FInputActionInstance& Instance);
	
	void FP_Sprint();

	void FP_StopSprinting();
	
	void FP_Interact();

	// ------ RTS FUNCTIONS ------

	void RTS_Move(const FInputActionInstance& Instance);

	void RTS_PanCamera(const FInputActionInstance& Instance);
	
	void RTS_RotateCamera(const FInputActionInstance& Instance);
	
	void RTS_MouseRotateCamera(const FInputActionInstance& Instance);
	
	void RTS_MouseInput(const FInputActionInstance& Instance);
	
	void RTS_Zoom(const FInputActionInstance& Instance);
	
	void RTS_Select();
	
	void RTS_Cancel();

	void RTS_RotateBuilding();
	
	void RTS_EquipRecycleTool();
	
	void ReturnToFirstPerson();

	// ------ TURRET FUNCTIONS ------

	void Turret_Look(const FInputActionInstance& Instance);

	void Turret_Fire();

	void Turret_Aim();
	
	void Turret_StopAiming();

	void Turret_Reload();

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	APlayerCharacter* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	ARTSCamera* GetRTSCamera();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Getters")
	EControllerMode GetControllerMode() { return ControllerMode; }

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	void SetControllerMode(EControllerMode NewMode);
	
};
