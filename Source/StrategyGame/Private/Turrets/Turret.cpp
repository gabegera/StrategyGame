// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets/Turret.h"

#include "Components/ArrowComponent.h"
#include "Components/ShootingComponent.h"


// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>("Turret Mesh");
	TurretMesh->SetupAttachment(StaticMeshComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Muzzle Location");
	ArrowComponent->SetupAttachment(TurretMesh);

	ShootingComponent = CreateDefaultSubobject<UShootingComponent>("Shooting");
}

void ATurret::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	MuzzleLocations.Empty();
    GetComponents<UArrowComponent>(MuzzleLocations);
	for (UArrowComponent* Muzzle : MuzzleLocations)
	{
		if (!Muzzle->IsAttachedTo(TurretMesh))
		{
			Muzzle->SetupAttachment(TurretMesh);
		}
	}
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();

	
}

void ATurret::Fire()
{
	for (int i = 0; i < MuzzleLocations.Num(); i++)
	{
		FVector ShotStart = MuzzleLocations[i]->GetComponentLocation();
		FVector ShotTarget = ShotStart + MuzzleLocations[i]->GetForwardVector() * 10000.0f;

		bool ShouldStartFireRateTimer;
		if (i == MuzzleLocations.Num() - 1)
		{
			ShouldStartFireRateTimer = true;
		}
		else
		{
			ShouldStartFireRateTimer = false;
		}
		ShootingComponent->Shoot(ShotStart, ShotTarget, ShouldStartFireRateTimer);
	}
}

void ATurret::Reload()
{
	ShootingComponent->StartReload();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

