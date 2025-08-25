// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Turrets/Turret.h"

#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ATurret::ATurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Skeletal Mesh");
	SkeletalMesh->SetupAttachment(SceneComponent);

	Arrow = CreateDefaultSubobject<UArrowComponent>("ProjectileSpawnPos");
	Arrow->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
}

void ATurret::Fire()
{
	if (GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;
	
	if (!InfiniteAmmo && AmmoInMagazine <= 0 && Ammo > 0) Reload();
	else if (!InfiniteAmmo && AmmoInMagazine <= 0) return;
	
	if (IsReloading()) return;

	ShootProjectileForward();
	
	if (!InfiniteAmmo) AmmoInMagazine -= 1;

	bool ShouldTimerLoop = false;
	GetWorldTimerManager().SetTimer(FireRateTimer, RPMToSeconds(FireRateRpm), ShouldTimerLoop);
}

void ATurret::Reload()
{
	
}

void ATurret::ShootProjectile(FVector Target)
{
	if (!IsValid(Projectile)) return;

	FVector TargetDirection = Target - Arrow->GetComponentLocation();
	TargetDirection.Normalize();

	if (ProjectileCount < 1) ProjectileCount = 1;
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);

		SpawnedProjectile->AddActorToIgnore(this);
		SpawnedProjectile->SetInstigator(GetInstigator());
		SpawnedProjectile->SetSpawner(this);
		SpawnedProjectile->SetActorLocation(Arrow->GetComponentLocation());
		SpawnedProjectile->GetProjectileMovement()->Velocity = TargetDirection * ProjectileSpeed;
		SpawnedProjectile->SetDamage(Damage);
		SpawnedProjectile->SetKnockbackForceMultiplier(KnockbackForceMultiplier);
	}
}

void ATurret::ShootProjectileForward()
{
	ShootProjectile(GetArrowComponent()->GetComponentLocation() + GetArrowComponent()->GetForwardVector() * 10000.0f);
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

