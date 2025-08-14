// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RemoteControlTurret.h"
#include "Actors/InteractableObject.h"
#include "RemoteTurretSeat.generated.h"

UCLASS()
class STRATEGYGAME_API ARemoteTurretSeat : public AInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARemoteTurretSeat();

protected:
	
	UPROPERTY(EditAnywhere)
	TArray<ARemoteControlTurret*> ConnectedTurrets;
	
	UPROPERTY()
	ARemoteControlTurret* ControlledTurret;

	// ------ ANIMATIONS ------

	UPROPERTY(EditAnywhere)
	UAnimationAsset* EnterAnimation = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteract(APlayerCharacter* InteractInstigator) override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
