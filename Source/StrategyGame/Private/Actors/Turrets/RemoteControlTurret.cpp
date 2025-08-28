// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Turrets/RemoteControlTurret.h"

ARemoteControlTurret::ARemoteControlTurret()
{
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Scene Capture");
	SceneCapture->SetupAttachment(SkeletalMesh);
}

void ARemoteControlTurret::Look(FVector2D Input)
{
	FRotator NewRotation = FRotator(FMath::Clamp(GetActorRotation().Pitch + Input.Y, -90.0f, 90.0f), GetActorRotation().Yaw + Input.X, GetActorRotation().Roll);
	SetActorRotation(NewRotation);
}

void ARemoteControlTurret::Fire()
{
	Super::Fire();
}
