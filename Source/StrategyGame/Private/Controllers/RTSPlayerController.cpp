// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/RTSPlayerController.h"

#include "GameplayDebuggerLocalController.h"
#include "Characters/PlayerCharacter.h"
#include "Pawns/RTSCamera.h"

void ARTSPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// FIRST PERSON CHARACTER INPUT
	Input->BindAction(Input_FP_Move, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Move);
	Input->BindAction(Input_FP_Move, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_Move);
	Input->BindAction(Input_FP_Look, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Look);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Sprint);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_StopSprinting);
	Input->BindAction(Input_FP_Interact, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Interact);

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

	// TURRET INPUT
	Input->BindAction(Input_Turret_Look, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Look);
	Input->BindAction(Input_Turret_Fire, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Fire);
	Input->BindAction(Input_Turret_Aim, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Aim);
	Input->BindAction(Input_Turret_Reload, ETriggerEvent::Triggered, this, &ARTSPlayerController::Turret_Reload);

	Input->BindAction(Input_ReturnToFirstPerson, ETriggerEvent::Triggered, this, &ARTSPlayerController::ReturnToFirstPerson);
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

void ARTSPlayerController::FP_Interact()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->TriggerInteraction();
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

void ARTSPlayerController::ReturnToFirstPerson()
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
		break;
	}
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
}

void ARTSPlayerController::Turret_Reload()
{
	if (ControllerMode != EControllerMode::Turret) return;
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
	case EControllerMode::FirstPerson:
		
		bEnableMouseOverEvents = false;
		bEnableClickEvents = false;
		SetShowMouseCursor(false);
		
		break;
	case EControllerMode::RTS:
		
		bEnableMouseOverEvents = true;
		bEnableClickEvents = true;
		SetShowMouseCursor(true);
		
		break;
	}

	OnControllerModeChangedDelegate.Broadcast();
}
