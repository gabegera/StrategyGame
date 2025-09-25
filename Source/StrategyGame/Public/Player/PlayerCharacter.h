// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "StrategyGameCharacter.h"
#include "Turrets/RemoteControlTurret.h"
#include "Camera/CameraComponent.h"
#include "Player/RTSPlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/InteractionInterface.h"
#include "PlayerCharacter.generated.h"

class ARTSCamera;

UCLASS()
class STRATEGYGAME_API APlayerCharacter : public AStrategyGameCharacter, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	
	UPROPERTY()
	ARTSPlayerController* RTSPlayerController;

	UPROPERTY(EditAnywhere, Category="Components")
	UCameraComponent* FirstPersonCamera = nullptr;

	UPROPERTY()
	float DefaultFOV;

	// ------ MOVEMENT ------

	UPROPERTY()
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category="Player Movement")
	float MaxSlowWalkSpeed;
	
	UPROPERTY(EditAnywhere, Category="Player Movement")
	float MaxSprintSpeed = 800.0f;

	// Controls how much slower the player will strafe while sprinting.
	UPROPERTY(EditAnywhere, Category="Player Movement")
	float SprintStrafeSpeedMultiplier = 0.2f;

	// Controls how fast the player's camera can turned while sprinting.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float MaximumSprintLookSpeed = 0.6f;

	UPROPERTY()
	float CrouchHeight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float ProneHeight;

	// ------ INTERACTION ------

	UPROPERTY()
	AActor* TargetInteractable = nullptr;
	
	UPROPERTY()
	FTimerHandle InteractionTimer;

	UPROPERTY()
	ARemoteControlTurret* ControlledTurret = nullptr;

	// How many times per second will there be a check for an interactable.
	UPROPERTY(EditAnywhere)
	int32 InteractionChecksPerSecond = 60.0f;
	
	UPROPERTY(EditAnywhere)
	float InteractionRange = 100.0f;

	UPROPERTY(EditAnywhere)
	float InteractionRadius = 32.0f;

	// ------ ANIMATION ------
	
	UPROPERTY(BlueprintReadWrite)
	UAnimationAsset* AnimationOverride = nullptr;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void Move(FVector2D MoveInput);
	
	UFUNCTION(BlueprintCallable)
	void Look(FVector2D LookInput);
	
	UFUNCTION(BlueprintCallable)
	void Sprint();
	
	UFUNCTION(BlueprintCallable)
	void StopSprinting();

	virtual void Crouch(bool bClientSimulation = false) override;

	virtual void UnCrouch(bool bClientSimulation = false) override;

	UFUNCTION(BlueprintCallable)
	void Prone();

	UFUNCTION(BlueprintCallable)
	void UnProne();

	UFUNCTION(BlueprintCallable)
	void SlowWalk();

	UFUNCTION(BlueprintCallable)
	void StopSlowWalking();

	virtual void UseEquippedItem() override;

	virtual void UseEquippedItemSecondary() override;

	virtual void EquipItemBySlot(EEquipmentSlot Slot) override;

	virtual void EquipItem(AEquippableItem* NewItem) override;

	virtual void HolsterEquippedItem() override;
	
	// Attempts to send an interact Interface Function to the target Interactable.
	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();
	
	UFUNCTION(BlueprintCallable)
	void CheckForInteractable();

	UFUNCTION(BlueprintCallable)
	void SwitchToRTSCam(ARTSCamera* TargetCamera);

	UFUNCTION(BlueprintCallable)
	void EnterSeat(AActor* Seat);

	UFUNCTION(BlueprintCallable)
	void Exit();

	UFUNCTION(BlueprintCallable)
	void ResetFOV();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenObjectiveMenu();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ARemoteControlTurret* SetControlledTurret(ARemoteControlTurret* NewTurret) { return ControlledTurret = NewTurret; }	

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARTSPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UCameraComponent* GetFirstPersonCamera() { return FirstPersonCamera; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARemoteControlTurret* GetControlledTurret() { return ControlledTurret; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsSprinting() { return GetCharacterMovement()->MaxWalkSpeed == MaxSprintSpeed; }

};
