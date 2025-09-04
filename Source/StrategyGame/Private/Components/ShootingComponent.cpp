// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShootingComponent.h"

#include "Projectile.h"


// Sets default values for this component's properties
UShootingComponent::UShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	AmmoInMagazine = MagazineCapacity;
}

void UShootingComponent::ShootProjectile(FVector ShotStart, FVector ShotTarget)
{
	ensureAlwaysMsgf(Projectile, TEXT("%s UShootingComponent::ShootProjectile failed due to Projectile being Null"), *GetOwner()->GetName());
	if (!Projectile) return;

	FVector TargetDirection = ShotTarget - ShotStart;
	TargetDirection.Normalize();

	if (ProjectileCount < 1) ProjectileCount = 1;
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);
		
		SpawnedProjectile->SetActorLocation(ShotStart);
		SpawnedProjectile->GetProjectileMovement()->Velocity = TargetDirection * ProjectileSpeed;
		SpawnedProjectile->SetDamage(Damage);
		SpawnedProjectile->SetKnockbackForceMultiplier(KnockbackForceMultiplier);
	}
}

void UShootingComponent::ShootHitscan(FVector ShotStart, FVector ShotTarget)
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, ShotStart, ShotTarget, ECC_Visibility);
	DrawDebugLine(GetWorld(), ShotStart, Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd, FColor::Red);

	if (Hit.GetActor())
	{
		Hit.GetActor()->TakeDamage(Damage, FDamageEvent(), nullptr, GetOwner());
	}
}

void UShootingComponent::Shoot(FVector ShotStart, FVector ShotTarget, bool ShouldStartFireRateTimer)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(FireRateTimer)) return;

	if (IsReloading()) return;
	if (!BottomlessClip && AmmoInMagazine <= 0) StartReload();

	if (Projectile)
	{
		ShootProjectile(ShotStart, ShotTarget);
	}
	else
	{
		ShootHitscan(ShotStart, ShotTarget);
	}
	
	if (!BottomlessClip) AmmoInMagazine -= 1;
	
	if (ShouldStartFireRateTimer)
	{
		bool ShouldTimerLoop = false;
		GetWorld()->GetTimerManager().SetTimer(FireRateTimer, RoundsPerMinuteToRoundsPerSecond(FireRateRoundsPerMinute), ShouldTimerLoop);
	}
	
}

void UShootingComponent::StartReload()
{
	if (ReloadTimeInSeconds <= 0)
	{
		CompleteReload();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ThisClass::CompleteReload, ReloadTimeInSeconds);
}

void UShootingComponent::CompleteReload()
{
	AmmoInMagazine = MagazineCapacity;
}

// Called every frame
void UShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

