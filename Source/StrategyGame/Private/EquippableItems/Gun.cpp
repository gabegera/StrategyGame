// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableItems/Gun.h"

#include "Components/ArrowComponent.h"
#include "Player/PlayerCharacter.h"


// Sets default values
AGun::AGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MuzzleArrowComponent = CreateDefaultSubobject<UArrowComponent>("Muzzle");
	MuzzleArrowComponent->SetupAttachment(SkeletalMeshComponent, TEXT("Grip_Bone"));

	ShootingComponent = CreateDefaultSubobject<UShootingComponent>("Shooting");
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGun::OnEquipped()
{
	Super::OnEquipped();

	
}

void AGun::OnUnEquipped()
{
	Super::OnUnEquipped();

	CancelReload();
	ShootingComponent->ResetFireRate();
}

void AGun::OnPickedUp()
{
	Super::OnPickedUp();

	
}

void AGun::OnDropped()
{
	Super::OnDropped();

	CancelReload();
	ShootingComponent->ResetFireRate();
}

void AGun::UseItemPrimary()
{
	Super::UseItemPrimary();

	Fire();
}

void AGun::ReleaseItemPrimary()
{
	Super::ReleaseItemPrimary();

	ShootingComponent->ResetFireRate();
}

void AGun::UseItemSecondary()
{
	Super::UseItemSecondary();

	StartAiming();
}

void AGun::ReleaseItemSecondary()
{
	Super::ReleaseItemSecondary();

	StopAiming();
}

void AGun::Fire()
{
	const FVector ShotStart = MuzzleArrowComponent->GetComponentLocation();
	const FVector ShotTarget = ShotStart + MuzzleArrowComponent->GetForwardVector() * 10000.0f;
	
	ShootingComponent->Shoot(ShotStart, ShotTarget, true);
}

void AGun::StartAiming()
{
	if (!ShootingComponent->IsAiming())
	{
		ZoomPlayerCamera(ShootingComponent->GetAimingZoomMultiplier());
	}
	
	ShootingComponent->StartAiming();
}

void AGun::StopAiming()
{
	ResetPlayerCameraZoom();
	
	ShootingComponent->StopAiming();
}

void AGun::Reload()
{
	ShootingComponent->StartReload();
}

void AGun::CancelReload()
{
	ShootingComponent->CancelReload();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

