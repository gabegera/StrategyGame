// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "RemoteControlTurret.generated.h"


UCLASS()
class STRATEGYGAME_API ARemoteControlTurret : public ATurret
{
	GENERATED_BODY()

public:
	ARemoteControlTurret();

protected:

	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* SceneCapture;

public:

	UFUNCTION(BlueprintCallable)
	void Look(FVector2D Input);

	virtual void Fire() override;
};
