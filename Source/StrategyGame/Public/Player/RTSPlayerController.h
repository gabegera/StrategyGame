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

UENUM(BlueprintType)
enum class EControllerMode : uint8
{
	FirstPerson UMETA(DisplayName="First Person Mode"),
	RTS			UMETA(DisplayName="RTS Mode"),
	Turret		UMETA(DisplayName="Turret Mode"),
	Terminal	UMETA(DisplayName="Terminal Mode")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FControllerModeChangedDelegate, EControllerMode, NewControllerMode);

UCLASS()
class STRATEGYGAME_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;
	
	UPROPERTY()
	ARTSCamera* RTSCamera;

	EControllerMode ControllerMode = EControllerMode::FirstPerson;

	// ------ INPUT ------
	
	// Default Mapping Context used for Enhanced Input.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> PlayerInputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Move;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Look;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Sprint;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Crouch;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Prone;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_SlowWalk;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_Interact;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_UseItem;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_UseItemSecondary;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_ReloadWeapon;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_EquipSlot1;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_EquipSlot2;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_EquipSlot3;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_EquipSlot4;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_EquipSlot5;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_HolsterItem;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_DropItem;
	UPROPERTY(EditAnywhere, Category = "Input|First Person") UInputAction* Input_FP_OpenObjectiveMenu;
	
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
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_1xSpeed;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_2xSpeed;
	UPROPERTY(EditAnywhere, Category = "Input|RTS") UInputAction* Input_RTS_3xSpeed;
	
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Look;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Fire;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Aim;
	UPROPERTY(EditAnywhere, Category = "Input|Turret") UInputAction* Input_Turret_Reload;

	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Exit;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float FP_MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float Turret_MouseSensitivity = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RTS_PanSensitivity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float RTS_RotateSensitivity = 1.0f;

	UPROPERTY()
	FVector2D MovementInput = FVector2D::ZeroVector;
	
	UPROPERTY()
	bool bIsPanBeingHeld = false;
	
	UPROPERTY()
	bool bIsMouseRotateBeingHeld = false;

	UFUNCTION(BlueprintCallable, Category="Input")
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnControllerModeChanged(EControllerMode NewControllerMode);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnControllerModeChanged")
	void BP_OnControllerModeChanged(EControllerMode NewControllerMode);

	UFUNCTION()
	void OnGamePaused();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnGamePaused")
	void BP_OnGamePaused();

	UFUNCTION()
	void OnGameUnPaused();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnGameUnPaused")
	void BP_OnGameUnPaused();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FControllerModeChangedDelegate OnControllerModeChangedDelegate;

	// Exits Turrets and RTS Mode, if the player isn't in either of those modes then it pauses the game.
	void Exit();	

	// ------ FIRST PERSON FUNCTIONS ------
	
	void FP_Move(const FInputActionInstance& Instance);
	
	void FP_Look(const FInputActionInstance& Instance);
	
	void FP_Sprint();

	void FP_StopSprinting();

	void FP_Crouch();

	void FP_Prone();

	void FP_SlowWalk();

	void FP_StopSlowWalking();
	
	void FP_Interact();

	void FP_UseItemPrimary();

	void FP_ReleaseItemPrimary();

	void FP_UseItemSecondary();

	void FP_ReleaseItemSecondary();

	void FP_ReloadWeapon();

	void FP_EquipSlot1();
	
	void FP_EquipSlot2();
	
	void FP_EquipSlot3();
	
	void FP_EquipSlot4();
	
	void FP_EquipSlot5();

	void FP_HolsterItem();

	void FP_DropItem();

	void FP_OpenObjectiveMenu();

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

	void RTS_Set1XSpeed();
	
	void RTS_Set2XSpeed();
	
	void RTS_Set3XSpeed();

	// ------ TURRET FUNCTIONS ------

	void Turret_Look(const FInputActionInstance& Instance);

	void Turret_Fire();

	void Turret_Aim();
	
	void Turret_StopAiming();

	void Turret_Reload();

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	APlayerCharacter* GetPlayerCharacter() { return PlayerCharacter; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	ARTSCamera* GetRTSCamera() { return RTSCamera; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Getters")
	EControllerMode GetControllerMode() { return ControllerMode; }

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	void SetControllerMode(EControllerMode NewMode);
	
};
