// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Turrets/RemoteControlTurret.h"

ARemoteControlTurret::ARemoteControlTurret()
{
	SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Scene Capture");
	SceneCapture->SetupAttachment(SkeletalMesh);
}

void ARemoteControlTurret::Look(FVector2D Input)
{
	FRotator NewRotation = FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + Input.X, FMath::Clamp(GetActorRotation().Roll + Input.Y, -90.0f, 90.0f));
	SetActorRotation(NewRotation);
}

void ARemoteControlTurret::Fire()
{
	Super::Fire();
}
