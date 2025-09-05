// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "RemoteControlTurret.generated.h"

UCLASS()
class STRATEGYGAME_API ARemoteControlTurret : public ATurret
{
	GENERATED_BODY()

public:
	ARemoteControlTurret();

protected:

	UPROPERTY(EditAnywhere, Category="Remote Controlled Turret")
	USceneCaptureComponent2D* SceneCapture;

	UPROPERTY(EditAnywhere, Category="Remote Controlled Turret")
	float MinVerticalRotation = -70.0f;

	UPROPERTY(EditAnywhere, Category="Remote Controlled Turret")
	float MaxVerticalRotation = 80.0f;

	UPROPERTY(VisibleAnywhere, Category="Remote Controlled Turret")
	float DefaultFOV = 0.0f;

	UPROPERTY(EditAnywhere, Category="Remote Controlled Turret")
	float ZoomFOV = 45.0f;

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void Look(FVector2D Input);

	UFUNCTION(BlueprintCallable)
	void Zoom();

	UFUNCTION(BlueprintCallable)
	void UnZoom();

	virtual void Fire() override;
};
