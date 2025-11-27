// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/StrategyPlayerController.h"

#include "StrategyStatics.h"
#include "EquippableItems/EquippableItem.h"
#include "Game/ResourcesSubsystem.h"
#include "Game/TimeSubsystem.h"
#include "Game/UnlocksSubsystem.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RTSCamera.h"

AStrategyPlayerController::AStrategyPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AStrategyPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	// FIRST PERSON CHARACTER INPUT
	Input->BindAction(Input_FP_Move, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Move);
	Input->BindAction(Input_FP_Move, ETriggerEvent::Completed, this, &AStrategyPlayerController::FP_Move);
	Input->BindAction(Input_FP_Look, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Look);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Sprint);
	Input->BindAction(Input_FP_Sprint, ETriggerEvent::Completed, this, &AStrategyPlayerController::FP_StopSprinting);
	Input->BindAction(Input_FP_Crouch, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Crouch);
	Input->BindAction(Input_FP_Prone, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Prone);
	Input->BindAction(Input_FP_SlowWalk, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_SlowWalk);
	Input->BindAction(Input_FP_SlowWalk, ETriggerEvent::Completed, this, &AStrategyPlayerController::FP_StopSlowWalking);
	Input->BindAction(Input_FP_Interact, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_Interact);
	Input->BindAction(Input_FP_UseItem, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_UseItemPrimary);
	Input->BindAction(Input_FP_UseItem, ETriggerEvent::Completed, this, &AStrategyPlayerController::FP_ReleaseItemPrimary);
	Input->BindAction(Input_FP_UseItemSecondary, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_UseItemSecondary);
	Input->BindAction(Input_FP_UseItemSecondary, ETriggerEvent::Completed, this, &AStrategyPlayerController::FP_ReleaseItemSecondary);
	Input->BindAction(Input_FP_ReloadWeapon, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_ReloadWeapon);
	Input->BindAction(Input_FP_EquipSlot1, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_EquipSlot1);
	Input->BindAction(Input_FP_EquipSlot2, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_EquipSlot2);
	Input->BindAction(Input_FP_EquipSlot3, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_EquipSlot3);
	Input->BindAction(Input_FP_EquipSlot4, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_EquipSlot4);
	Input->BindAction(Input_FP_EquipSlot5, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_EquipSlot5);
	Input->BindAction(Input_FP_HolsterItem, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_HolsterItem);
	Input->BindAction(Input_FP_DropItem, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_DropItem);
	Input->BindAction(Input_FP_OpenObjectiveMenu, ETriggerEvent::Triggered, this, &AStrategyPlayerController::FP_OpenObjectiveMenu);

	// RTS CAMERA INPUT
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_Move, ETriggerEvent::Completed, this, &AStrategyPlayerController::RTS_Move);
	Input->BindAction(Input_RTS_PanCamera, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_PanCamera);
	Input->BindAction(Input_RTS_PanCamera, ETriggerEvent::Completed, this, &AStrategyPlayerController::RTS_PanCamera);
	Input->BindAction(Input_RTS_RotateCamera, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_RotateCamera);
	Input->BindAction(Input_RTS_RotateCamera, ETriggerEvent::Completed, this, &AStrategyPlayerController::RTS_RotateCamera);
	Input->BindAction(Input_RTS_Mouse_RotateCamera, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_MouseRotateCamera);
	Input->BindAction(Input_RTS_Mouse_RotateCamera, ETriggerEvent::Completed, this, &AStrategyPlayerController::RTS_MouseRotateCamera);
	Input->BindAction(Input_RTS_MouseInput, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_MouseInput);
	Input->BindAction(Input_RTS_MouseInput, ETriggerEvent::Completed, this, &AStrategyPlayerController::RTS_MouseInput);
	Input->BindAction(Input_RTS_Zoom, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Zoom);
	Input->BindAction(Input_RTS_Select, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Select);
	Input->BindAction(Input_RTS_Cancel, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Cancel);
	Input->BindAction(Input_RTS_RotateBuilding, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_RotateBuilding);
	Input->BindAction(Input_RTS_EquipRecycleTool, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_EquipRecycleTool);
	Input->BindAction(Input_RTS_1xSpeed, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Set1XSpeed);
	Input->BindAction(Input_RTS_2xSpeed, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Set2XSpeed);
	Input->BindAction(Input_RTS_3xSpeed, ETriggerEvent::Triggered, this, &AStrategyPlayerController::RTS_Set3XSpeed);

	// TURRET INPUT
	Input->BindAction(Input_Turret_Look, ETriggerEvent::Triggered, this, &AStrategyPlayerController::Turret_Look);
	Input->BindAction(Input_Turret_Fire, ETriggerEvent::Triggered, this, &AStrategyPlayerController::Turret_Fire);
	Input->BindAction(Input_Turret_Aim, ETriggerEvent::Triggered, this, &AStrategyPlayerController::Turret_Aim);
	Input->BindAction(Input_Turret_Aim, ETriggerEvent::Completed, this, &AStrategyPlayerController::Turret_StopAiming);
	Input->BindAction(Input_Turret_Reload, ETriggerEvent::Triggered, this, &AStrategyPlayerController::Turret_Reload);

	Input->BindAction(Input_Exit, ETriggerEvent::Triggered, this, &AStrategyPlayerController::Exit);
}

void AStrategyPlayerController::PrintDebugInfo()
{
	if (bPrintDebugInfo)
	{
		int32 DebugMessageKey = 10;

		if (bPrintPlayerDebug && GetPlayerCharacter())
		{
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::Green, "Player: ");

			UHealthComponent* PlayerHealthComponent = GetPlayerCharacter()->GetComponentByClass<UHealthComponent>();
			FString PlayerHealthString = "Health: ";
			PlayerHealthString.Append(FString::SanitizeFloat(PlayerHealthComponent->GetHealth()));
			PlayerHealthString.Append(" / ");
			PlayerHealthString.Append(FString::SanitizeFloat(PlayerHealthComponent->GetMaxHealth()));
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, PlayerHealthString);

			// Blank Space
			GEngine->AddOnScreenDebugMessage(DebugMessageKey, 1.0f, FColor::White, " ");
		}

		if (bPrintTimeDebug)
		{
			UTimeSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UTimeSubsystem>();

			DebugMessageKey = 20;

			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::Yellow, "Time: ");

			FString DaysSurvivedString = "Day ";
			DaysSurvivedString.AppendInt(TimeSubsystem->GetDaysCityHasSurvived());
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, DaysSurvivedString);

			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, TimeSubsystem->GetTimeOfDayString());

			FString IsWorkTimeString = "Is Work Time: ";
			IsWorkTimeString.Append(TimeSubsystem->IsTimeToWork() ? "True" : "False");
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, IsWorkTimeString);

			// Blank Space
			GEngine->AddOnScreenDebugMessage(DebugMessageKey, 1.0f, FColor::White, " ");
		}

		if (bPrintPopulationDebug)
		{

		}

		if (bPrintResourcesDebug)
		{
			UResourcesSubsystem* ResourcesSubsystem = GetGameInstance()->GetSubsystem<UResourcesSubsystem>();
			DebugMessageKey = 30;
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::Orange, "Resources: ");
			TArray<UResourceDataAsset*> Resources = UStrategyStatics::GetAllResources();
			Resources.Sort([](const UResourceDataAsset& A, const UResourceDataAsset& B)
			{
				return A.GetResourceName().ToString() < B.GetResourceName().ToString();
			});
			for (UResourceDataAsset* Resource : Resources)
			{
				FString ResourceString = Resource->GetResourceName().ToString();
				ResourceString.Append(": ");
				ResourceString.Append(FString::SanitizeFloat(ResourcesSubsystem->GetResourceAmount(Resource)));
				ResourceString.Append(" / ");
				ResourceString.AppendInt(ResourcesSubsystem->GetResourceCapacity(Resource));
				GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, ResourceString);
			}

			// Blank Space
			GEngine->AddOnScreenDebugMessage(DebugMessageKey, 1.0f, FColor::White, " ");
		}

		if (bPrintUpgradesDebug)
		{
			UUnlocksSubsystem* UpgradesSubsystem = GetGameInstance()->GetSubsystem<UUnlocksSubsystem>();
			DebugMessageKey = 40;
			GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::Cyan, "Unlocked Upgrades:");

			if (UpgradesSubsystem->GetUnlockedUpgrades().IsEmpty())
			{
				GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, "No Upgrades Unlocked");
			}
			else
			{
				for (const UUpgradeDataAsset* Upgrade : UpgradesSubsystem->GetUnlockedUpgrades())
				{
					GEngine->AddOnScreenDebugMessage(DebugMessageKey++, 1.0f, FColor::White, Upgrade->GetTitle());
				}
			}

			// Blank Space
			GEngine->AddOnScreenDebugMessage(DebugMessageKey, 1.0f, FColor::White, " ");
		}
	}
}

void AStrategyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AStrategyPlayerController::BeginPlay()
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

	PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	RTSCamera = Cast<ARTSCamera>(UGameplayStatics::GetActorOfClass(this, ARTSCamera::StaticClass()));

	OnControllerModeChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnControllerModeChanged);
}

void AStrategyPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn->IsA(ARTSCamera::StaticClass()))
	{
		RTSCamera = Cast<ARTSCamera>(InPawn);
		SetControllerMode(EControllerMode::RTS);
	}
	else if (InPawn->IsA(APlayerCharacter::StaticClass()))
	{
		PlayerCharacter = Cast<APlayerCharacter>(InPawn);
		SetControllerMode(EControllerMode::FirstPerson);
	}
}

void AStrategyPlayerController::OnControllerModeChanged(EControllerMode NewControllerMode)
{
	BP_OnControllerModeChanged(NewControllerMode);
}

void AStrategyPlayerController::OnGamePaused()
{
	BP_OnGamePaused();
}

void AStrategyPlayerController::OnGameUnPaused()
{
	BP_OnGameUnPaused();
}

void AStrategyPlayerController::Exit()
{
	switch (ControllerMode)
	{
	case EControllerMode::RTS:
		GetRTSCamera()->ExitRTSMode();
		break;
	case EControllerMode::Turret:
		GetPlayerCharacter()->Exit();
		break;
	case EControllerMode::Terminal:
		GetPlayerCharacter()->Exit();
		break;
	default:
		if (IsPaused())
		{
			SetPause(false);
		}
		else
		{
			SetPause(true);
		}
		break;
	}
}

void AStrategyPlayerController::FP_Move(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	MovementInput = Value;
	
	GetPlayerCharacter()->Move(Value);
}

void AStrategyPlayerController::FP_Look(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();

	Value = Value * FP_MouseSensitivity;
	
	GetPlayerCharacter()->Look(Value * FP_MouseSensitivity);
}

void AStrategyPlayerController::FP_Sprint()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->Sprint();
}

void AStrategyPlayerController::FP_StopSprinting()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->StopSprinting();
}

void AStrategyPlayerController::FP_Crouch()
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

void AStrategyPlayerController::FP_Prone()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
}

void AStrategyPlayerController::FP_SlowWalk()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->SlowWalk();
}

void AStrategyPlayerController::FP_StopSlowWalking()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->StopSlowWalking();
}

void AStrategyPlayerController::FP_Interact()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;
	
	GetPlayerCharacter()->TriggerInteraction();
}

void AStrategyPlayerController::FP_UseItemPrimary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->UseEquippedItem();
}

void AStrategyPlayerController::FP_ReleaseItemPrimary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReleaseEquippedItem();
}

void AStrategyPlayerController::FP_UseItemSecondary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->UseEquippedItemSecondary();
}

void AStrategyPlayerController::FP_ReleaseItemSecondary()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReleaseEquippedItemSecondary();
}

void AStrategyPlayerController::FP_ReloadWeapon()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->ReloadEquippedItem();
}

void AStrategyPlayerController::FP_EquipSlot1()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot1);
}

void AStrategyPlayerController::FP_EquipSlot2()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot2);
}

void AStrategyPlayerController::FP_EquipSlot3()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot3);
}

void AStrategyPlayerController::FP_EquipSlot4()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot4);
}

void AStrategyPlayerController::FP_EquipSlot5()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->EquipItemBySlot(EEquipmentSlot::Slot5);
}

void AStrategyPlayerController::FP_HolsterItem()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->HolsterEquippedItem();
}

void AStrategyPlayerController::FP_DropItem()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->DropEquippedItem();
}

void AStrategyPlayerController::FP_OpenObjectiveMenu()
{
	if (ControllerMode != EControllerMode::FirstPerson) return;

	GetPlayerCharacter()->OpenObjectiveMenu();
}

void AStrategyPlayerController::RTS_Move(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	
	GetRTSCamera()->Move(Value);
}

void AStrategyPlayerController::RTS_PanCamera(const FInputActionInstance& Instance)
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

void AStrategyPlayerController::RTS_RotateCamera(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;

	float Value = Instance.GetValue().Get<float>();
	
	GetRTSCamera()->RotateCamera(Value);
}

void AStrategyPlayerController::RTS_MouseRotateCamera(const FInputActionInstance& Instance)
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

void AStrategyPlayerController::RTS_MouseInput(const FInputActionInstance& Instance)
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

void AStrategyPlayerController::RTS_Zoom(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::RTS) return;
	
	float Value = Instance.GetValue().Get<float>();

	GetRTSCamera()->Zoom(Value);
}

void AStrategyPlayerController::RTS_Select()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->SelectTarget();
}

void AStrategyPlayerController::RTS_Cancel()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->CancelAction();
}

void AStrategyPlayerController::RTS_RotateBuilding()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->RotateStructure();
}

void AStrategyPlayerController::RTS_EquipRecycleTool()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetRTSCamera()->EquipRecycleTool();
}

void AStrategyPlayerController::RTS_Set1XSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetGameInstance()->GetSubsystem<UTimeSubsystem>()->SetTimeScaleMultiplier(1.0f);
}

void AStrategyPlayerController::RTS_Set2XSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetGameInstance()->GetSubsystem<UTimeSubsystem>()->SetTimeScaleMultiplier(2.0f);
}

void AStrategyPlayerController::RTS_Set3XSpeed()
{
	if (ControllerMode != EControllerMode::RTS) return;

	GetGameInstance()->GetSubsystem<UTimeSubsystem>()->SetTimeScaleMultiplier(3.0f);
}

void AStrategyPlayerController::Turret_Look(const FInputActionInstance& Instance)
{
	if (ControllerMode != EControllerMode::Turret) return;

	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	Value *= Turret_MouseSensitivity;

	GetPlayerCharacter()->GetControlledTurret()->Look(Value);
}

void AStrategyPlayerController::Turret_Fire()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->Fire();
}

void AStrategyPlayerController::Turret_Aim()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->Zoom();
}

void AStrategyPlayerController::Turret_StopAiming()
{
	if (ControllerMode != EControllerMode::Turret) return;

	GetPlayerCharacter()->GetControlledTurret()->UnZoom();
}

void AStrategyPlayerController::Turret_Reload()
{
	if (ControllerMode != EControllerMode::Turret) return;
}

void AStrategyPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	PrintDebugInfo();
}

APlayerCharacter* AStrategyPlayerController::GetPlayerCharacter() const
{
	return PlayerCharacter;
}

ARTSCamera* AStrategyPlayerController::GetRTSCamera() const
{
	return RTSCamera;
}

EControllerMode AStrategyPlayerController::GetControllerMode() const
{
	return ControllerMode;
}

void AStrategyPlayerController::SetControllerMode(const EControllerMode NewMode)
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
