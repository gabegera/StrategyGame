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
	Input->BindAction(Input_FP_Move, ETriggerEvent::Canceled, this, &ARTSPlayerController::FP_Move);
	Input->BindAction(Input_FP_Look, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Look);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Sprint);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Completed, this, &ARTSPlayerController::FP_StopSprinting);
	Input->BindAction(Input_FP_Interact, ETriggerEvent::Triggered, this, &ARTSPlayerController::FP_Interact);

	// RTS CAMERA INPUT
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Canceled, this, &ARTSPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_Zoom, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Zoom);
	Input->BindAction(Input_RTS_Select, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Select);
	Input->BindAction(Input_RTS_Cancel, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_Cancel);
	Input->BindAction(Input_RTS_ExitMode, ETriggerEvent::Triggered, this, &ARTSPlayerController::RTS_ExitMode);
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
	
	GetPlayerCharacter()->Interact();
}

void ARTSPlayerController::RTS_Move(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	
	GetRTSCamera()->Move(Value);
}

void ARTSPlayerController::RTS_MouseInput()
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	GetRTSCamera()->MouseInput();
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

void ARTSPlayerController::RTS_ExitMode()
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	GetRTSCamera()->ExitRTSMode();
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
