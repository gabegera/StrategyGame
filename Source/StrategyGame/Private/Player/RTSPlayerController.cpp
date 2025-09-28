// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RTSPlayerController.h"

#include "PlayerCharacterCheats.h"
#include "EquippableItems/EquippableItem.h"
#include "GameFramework/CheatManager.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RTSCamera.h"

void ARTSPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// FIRST PERSON CHARACTER INPUT
	Input->BindAction(Input_FP_Move, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Move);
	Input->BindAction(Input_FP_Move, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_Move);
	Input->BindAction(Input_FP_Look, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Look);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Sprint);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_StopSprinting);
	Input->BindAction(Input_FP_Crouch, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Crouch);
	Input->BindAction(Input_FP_Prone, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Prone);
	Input->BindAction(Input_FP_SlowWalk, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_SlowWalk);
	Input->BindAction(Input_FP_SlowWalk, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_StopSlowWalking);
	Input->BindAction(Input_FP_Interact, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Interact);
	Input->BindAction(Input_FP_UseItem, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_UseItemPrimary);
	Input->BindAction(Input_FP_UseItem, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_ReleaseItemPrimary);
	Input->BindAction(Input_FP_UseItemSecondary, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_UseItemSecondary);
	Input->BindAction(Input_FP_UseItemSecondary, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_ReleaseItemSecondary);
	Input->BindAction(Input_FP_ReloadWeapon, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_ReloadWeapon);
	Input->BindAction(Input_FP_EquipSlot1, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_EquipSlot1);
	Input->BindAction(Input_FP_EquipSlot2, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_EquipSlot2);
	Input->BindAction(Input_FP_EquipSlot3, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_EquipSlot3);
	Input->BindAction(Input_FP_EquipSlot4, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_EquipSlot4);
	Input->BindAction(Input_FP_EquipSlot5, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_EquipSlot5);
	Input->BindAction(Input_FP_HolsterItem, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_HolsterItem);
	Input->BindAction(Input_FP_DropItem, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_DropItem);
	Input->BindAction(Input_FP_OpenObjectiveMenu, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_OpenObjectiveMenu);

	// RTS CAMERA INPUT
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Completed, this, &ARTSPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_PanCamera, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_PanCamera);
	Input->BindAction(Input_RTS_PanCamera, ETriggerEvent::Completed, this, &ARTSPlayerController::RTS_PanCamera);
	Input->BindAction(Input_RTS_RotateCamera, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_RotateCamera);
	Input->BindAction(Input_RTS_RotateCamera, ETriggerEvent::Completed, this, &ARTSPlayerController::RTS_RotateCamera);
	Input->BindAction(Input_RTS_Mouse_RotateCamera, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_MouseRotateCamera);
	Input->BindAction(Input_RTS_Mouse_RotateCamera, ETriggerEvent::Completed, this, &ARTSPlayerController::RTS_MouseRotateCamera);
	Input->BindAction(Input_RTS_MouseInput, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_MouseInput);
	Input->BindAction(Input_RTS_MouseInput, ETriggerEvent::Completed, this, &ARTSPlayerController::RTS_MouseInput);
	Input->BindAction(Input_RTS_Zoom, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Zoom);
	Input->BindAction(Input_RTS_Select, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Select);
	Input->BindAction(Input_RTS_Cancel, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Cancel);
	Input->BindAction(Input_RTS_RotateBuilding, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_RotateBuilding);
	Input->BindAction(Input_RTS_EquipRecycleTool, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_EquipRecycleTool);
	Input->BindAction(Input_RTS_1xSpeed, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Set1xSpeed);
	Input->BindAction(Input_RTS_2xSpeed, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Set2xSpeed);
	Input->BindAction(Input_RTS_3xSpeed, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Set3xSpeed);

	// TURRET INPUT
	Input->BindAction(Input_Turret_Look, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Look);
	Input->BindAction(Input_Turret_Fire, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Fire);
	Input->BindAction(Input_Turret_Aim, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Aim);
	Input->BindAction(Input_Turret_Aim, ETriggerEvent::Completed, this, &ARTSPlayerController::Turret_StopAiming);
	Input->BindAction(Input_Turret_Reload, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Reload);

	Input->BindAction(Input_Exit, ETriggerEvent::Triggered, this, &ARTSPlayerController::Exit);
}

void ARTSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AddCheats(true);
	CheatManager->AddCheatManagerExtension(NewObject<UPlayerCharacterCheats>(UPlayerCharacterCheats::StaticClass()));
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
    {
    	if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
    	{
    		InputSystem->ClearAllMappings();

    		InputSystem->AddMappingContext(PlayerInputMapping.LoadSynchronous(), 0);
    	}

    	SetupPlayerInputComponent(InputComponent);
    }

	OnControllerModeChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnControllerModeChanged);
	OnGamePausedDelegate.AddUniqueDynamic(this, &ThisClass::OnGamePaused);
	OnGameUnPausedDelegate.AddUniqueDynamic(this, &ThisClass::OnGameUnPaused);
	GetStrategyGameState()->OnTimeScaleChanged.AddUniqueDynamic(this, &ThisClass::OnTimeScaleChanged);
}

void ARTSPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn->IsA(ARTSCamera::StaticClass()))
	{
		GetRTSCamera(); // Used to assign object pointer.
		SetControllerMode(EControllerMode::RTS);
	}
	else if (InPawn->IsA(APlayerCharacter::StaticClass()))
	{
		GetPlayerCharacter(); // Used to assign object pointer.
		SetControllerMode(EControllerMode::FirstPerson);
	}
}

void ARTSPlayerController::OnTimeScaleChanged(const ETimeScale NewTimeScale)
{
	switch (NewTimeScale)
	{
	case ETimeScale::OneTimesSpeed:
		CustomTimeDilation = 1.0f;
		break;
	case ETimeScale::TwoTimesSpeed:
		CustomTimeDilation = 1.0f / 2.0f;
		break;
	case ETimeScale::ThreeTimesSpeed:
		CustomTimeDilation = 1.0f / 3.0f;
		break;
	}
	
	BP_OnTimeScaleChanged(NewTimeScale);
}

void ARTSPlayerController::OnControllerModeChanged(EControllerMode NewControllerMode)
{
	BP_OnControllerModeChanged(NewControllerMode);
}

void ARTSPlayerController::OnGamePaused()
{
	BP_OnGamePaused();
}

void ARTSPlayerController::OnGameUnPaused()
{
	BP_OnGameUnPaused();
}

void ARTSPlayerController::Exit()
{
	switch (ControllerMode)
	{
	case EControllerMode::RTS:
		GetRTSCamera()->ExitRTSMode();
		break;
	case EControllerMode::Turret:
		GetPlayerCharacter()->Exit();
		break;
	default:
		if (IsPaused())
		{
			SetPause(false);
			OnGameUnPausedDelegate.Broadcast();
		}
		else
		{
			SetPause(true);
			OnGamePausedDelegate.Broadcast();
		}
		break;
	}
}

void ARTSPlayerController::FP_Move(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	MovementInput = Value;
	
	GetPlayerCharacter()->Move(Value);
}

void ARTSPlayerController::FP_Look(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	Value = Value * FP_MouseSensitivity;
	
	GetPlayerCharacter()->Look(Value * FP_MouseSensitivity);
}

void ARTSPlayerController::FP_Sprint()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->Sprint();
}

void ARTSPlayerController::FP_StopSprinting()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->StopSprinting();
}

void ARTSPlayerController::FP_Crouch()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	if (GetPlayerCharacter()->IsCrouched())
	{
		GetPlayerCharacter()->UnCrouch();
	}
	else
	{
		GetPlayerCharacter()->Crouch();
	}
}

void ARTSPlayerController::FP_Prone()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
}

void ARTSPlayerController::FP_SlowWalk()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->SlowWalk();
}

void ARTSPlayerController::FP_StopSlowWalking()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->StopSlowWalking();
}

void ARTSPlayerController::FP_Interact()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->TriggerInteraction();
}

void ARTSPlayerController::FP_UseItemPrimary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->UseEquippedItem();
}

void ARTSPlayerController::FP_ReleaseItemPrimary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReleaseEquippedItem();
}

void ARTSPlayerController::FP_UseItemSecondary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->UseEquippedItemSecondary();
}

void ARTSPlayerController::FP_ReleaseItemSecondary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReleaseEquippedItemSecondary();
}

void ARTSPlayerController::FP_ReloadWeapon()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReloadEquippedItem();
}

void ARTSPlayerController::FP_EquipSlot1()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot1);
}

void ARTSPlayerController::FP_EquipSlot2()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot2);
}

void ARTSPlayerController::FP_EquipSlot3()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot3);
}

void ARTSPlayerController::FP_EquipSlot4()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot4);
}

void ARTSPlayerController::FP_EquipSlot5()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot5);
}

void ARTSPlayerController::FP_HolsterItem()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->HolsterEquippedItem();
}

void ARTSPlayerController::FP_DropItem()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->DropEquippedItem();
}

void ARTSPlayerController::FP_OpenObjectiveMenu()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->OpenObjectiveMenu();
}

void ARTSPlayerController::RTS_Move(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	
	GetRTSCamera()->Move(Value);
}

void ARTSPlayerController::RTS_PanCamera(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	bIsPanBeingHeld = Instance.GetValue().Get<bool>();

	if (bIsPanBeingHeld)
	{
		bEnableMouseOverEvents = false;
        bEnableClickEvents = false;
        SetShowMouseCursor(false);
	}
	else
	{
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		SetShowMouseCursor(true);
	}
}

void ARTSPlayerController::RTS_RotateCamera(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;

	float Value = Instance.GetValue().Get<float>();
	
	GetRTSCamera()->RotateCamera(Value);
}

void ARTSPlayerController::RTS_MouseRotateCamera(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;

	bIsMouseRotateBeingHeld = Instance.GetValue().Get<bool>();

	if (bIsMouseRotateBeingHeld)
	{
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;
		SetShowMouseCursor(false);
	}
	else
	{
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		SetShowMouseCursor(true);
	}
}

void ARTSPlayerController::RTS_MouseInput(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;

	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	Value = FVector2D(-Value.X, Value.Y);

	if (bIsPanBeingHeld)
	{
		GetRTSCamera()->Move(Value * RTS_PanSensitivity);
	}

	if (bIsMouseRotateBeingHeld)
	{
		GetRTSCamera()->RotateCamera(-Value.X * RTS_RotateSensitivity);
	}
}

void ARTSPlayerController::RTS_Zoom(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	float Value = Instance.GetValue().Get<float>();

	GetRTSCamera()->Zoom(Value);
}

void ARTSPlayerController::RTS_Select()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->SelectTarget();
}

void ARTSPlayerController::RTS_Cancel()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->CancelAction();
}

void ARTSPlayerController::RTS_RotateBuilding()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->RotateBuilding();
}

void ARTSPlayerController::RTS_EquipRecycleTool()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->EquipRecycleTool();
}

void ARTSPlayerController::RTS_Set1xSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetStrategyGameState()->SetTimeScale(ETimeScale::OneTimesSpeed);
}

void ARTSPlayerController::RTS_Set2xSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetStrategyGameState()->SetTimeScale(ETimeScale::TwoTimesSpeed);
}

void ARTSPlayerController::RTS_Set3xSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetStrategyGameState()->SetTimeScale(ETimeScale::ThreeTimesSpeed);
}

void ARTSPlayerController::Turret_Look(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::Turret) return;

	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	Value *= Turret_MouseSensitivity;

	GetPlayerCharacter()->GetControlledTurret()->Look(Value);
}

void ARTSPlayerController::Turret_Fire()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->Fire();
}

void ARTSPlayerController::Turret_Aim()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->Zoom();
}

void ARTSPlayerController::Turret_StopAiming()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->UnZoom();
}

void ARTSPlayerController::Turret_Reload()
{
	if (ControllerMode != EControllerMode::Turret) return;
}

AStrategyGameState* ARTSPlayerController::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

AStrategyGameModeBase* ARTSPlayerController::GetStrategyGameMode()
{
	if (StrategyGameMode == nullptr)
	{
		StrategyGameMode = Cast<AStrategyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	return StrategyGameMode;
}

APlayerCharacter* ARTSPlayerController::GetPlayerCharacter()
{
	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
	}

	return PlayerCharacter;
}

ARTSCamera* ARTSPlayerController::GetRTSCamera()
{
	if (RTSCamera == nullptr)
	{
		RTSCamera = Cast<ARTSCamera>(GetPawn());
	}

	return RTSCamera;
}

void ARTSPlayerController::SetControllerMode(EControllerMode NewMode)
{
	ControllerMode = NewMode;

	switch (ControllerMode)
	{
	case EControllerMode::RTS:
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		SetShowMouseCursor(true);
		
		break;
		
	default:
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;
		SetShowMouseCursor(false);
		
		break;
	}

	OnControllerModeChangedDelegate.Broadcast(NewMode);
}
