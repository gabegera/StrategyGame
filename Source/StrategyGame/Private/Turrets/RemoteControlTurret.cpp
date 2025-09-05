// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets/RemoteControlTurret.h"

#include "Components/SceneCaptureComponent2D.h"

ARemoteControlTurret::ARemoteControlTurret()
{
	BuildingBounds->SetVisibility(false);
	
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Scene Capture");
	SceneCapture->SetupAttachment(TurretMesh);
}

void ARemoteControlTurret::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = SceneCapture->FOVAngle;
}

void ARemoteControlTurret::Look(FVector2D Input)
{
	Input /= DefaultFOV / SceneCapture->FOVAngle;
	FRotator NewRotation = FRotator(FMath::Clamp(TurretMesh->GetComponentRotation().Pitch + Input.Y, MinVerticalRotation, MaxVerticalRotation),
		TurretMesh->GetComponentRotation().Yaw + Input.X, TurretMesh->GetComponentRotation().Roll);
	
	TurretMesh->SetWorldRotation(NewRotation);
}

void ARemoteControlTurret::Zoom()
{
	SceneCapture->FOVAngle = ZoomFOV;
}

void ARemoteControlTurret::UnZoom()
{
	SceneCapture->FOVAngle = DefaultFOV;
}

void ARemoteControlTurret::Fire()
{
	Super::Fire();
}
