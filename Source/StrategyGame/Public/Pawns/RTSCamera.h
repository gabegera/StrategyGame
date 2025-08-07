// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BuildableStructure.h"
#include "Camera/CameraComponent.h"
#include "Controllers/RTSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "RTSCamera.generated.h"

class AStrategyGameState;
class APlayerCharacter;

UCLASS()
class STRATEGYGAME_API ARTSCamera : public APawn, public ISelectionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARTSCamera();

protected:

	FOnStructureSelectedDelegate OnStructureSelectedDelegate;

	UPROPERTY() ARTSPlayerController* RTSPlayerController = nullptr;
	UPROPERTY() AStrategyGameState* StrategyGameState = nullptr;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera = nullptr;

	// ------ MOVEMENT ------

	// UPROPERTY() float MoveSpeed;
	
	UPROPERTY(EditAnywhere)
	float MaxMoveSpeed = 5000.0f;

	// UPROPERTY(EditAnywhere)
	// float MovementAcceleration = 500.0f;

	UPROPERTY(EditAnywhere)
	float MaxZoomSpeed = 2000.0f;
	
	UPROPERTY(EditAnywhere)
	float MaxZoomHeight = 10000.0f;

	UPROPERTY(EditAnywhere)
	float MinZoomHeight = 4000.0f;

	// ------ STRUCTURE BUILDING ------

	// The Structure that has been selected to be built.
	UPROPERTY() ABuildableStructure* StructureToBuild;
	// The Structure that has been selected / clicked on.
	UPROPERTY() ABuildableStructure* SelectedStructure;

	UPROPERTY(EditAnywhere, Category="Structure Building")
	int32 SnappingSize = 50;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION()
	virtual void OnStructureSelected(ABuildableStructure* Selection);

public:
	
	void Move(FVector2D MoveInput);
	
	void MouseInput();
	
	void Zoom(float Input);
	
	void SelectTarget();
	
	void CancelAction();
	
	void BuildStructure();

	UFUNCTION(BlueprintCallable)
	void ExitRTSMode();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FHitResult LineTraceToMousePos();

	UFUNCTION(BlueprintCallable)
	void SelectStructureBlueprint(TSubclassOf<ABuildableStructure> NewBlueprint);

	UFUNCTION(BlueprintCallable)
	void MoveStructureToMousePos();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARTSPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ABuildableStructure* GetSelectedStructure() { return StructureToBuild; }

};
