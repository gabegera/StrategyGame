// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets/RemoteTurretSeat.h"

#include "Player/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ARemoteTurretSeat::ARemoteTurretSeat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARemoteTurretSeat::BeginPlay()
{
	Super::BeginPlay();

	if (!ConnectedTurrets.IsEmpty())
	{
		ControlledTurret = ConnectedTurrets[0];
	}
}

void ARemoteTurretSeat::OnInteract(APlayerCharacter* InteractInstigator)
{
	Super::OnInteract(InteractInstigator);

	if (!ControlledTurret) return;

	InteractInstigator->SetControlledTurret(ControlledTurret);
	InteractInstigator->GetPlayerController()->SetControllerMode(EControllerMode::Turret);
	InteractInstigator->EnterSeat(this);
}

// Called every frame
void ARemoteTurretSeat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

