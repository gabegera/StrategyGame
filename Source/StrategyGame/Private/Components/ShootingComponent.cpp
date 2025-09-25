// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ShootingComponent.h"

#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"


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

	if (ProjectileCount < 1) ProjectileCount = 1;
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);

		if (IsAiming())
		{
			TargetDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetDirection, MaxAimingSpreadDegrees * 2);
		}
		else
		{
			TargetDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(TargetDirection, MaxHipfireSpreadDegrees * 2);
		}		
		
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
	
	if (bIsSemiAuto && bIsTriggerPulled) return;
	
	if (IsReloading()) return;
	
	if (!bInfiniteAmmo && AmmoInMagazine <= 0)
	{
		StartReload();
		return;
	}

	if (Projectile)
	{
		ShootProjectile(ShotStart, ShotTarget);
	}
	else
	{
		ShootHitscan(ShotStart, ShotTarget);
	}
	
	if (!bInfiniteAmmo) AmmoInMagazine -= 1;
	
	if (ShouldStartFireRateTimer)
	{
		bool bShouldTimerLoop = false;
		GetWorld()->GetTimerManager().SetTimer(FireRateTimer, RoundsPerMinuteToRoundsPerSecond(FireRateRoundsPerMinute), bShouldTimerLoop);
	}

	bIsTriggerPulled = true;
}

void UShootingComponent::StartAiming()
{
	bIsAiming = true;
}

void UShootingComponent::StopAiming()
{
	bIsAiming = false;
}

void UShootingComponent::StartReload()
{
	if (IsReloading()) return;

	if (AmmoInMagazine == MagazineCapacity) return;
	
	if (ReloadTimeInSeconds <= 0)
	{
		CompleteReload();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ThisClass::CompleteReload, ReloadTimeInSeconds);
}

void UShootingComponent::CancelReload()
{
	if (IsReloading())
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimer);
	}
}

void UShootingComponent::ResetFireRate()
{
	bIsTriggerPulled = false;

	GetWorld()->GetTimerManager().ClearTimer(FireRateTimer);
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

	if (IsReloading())
	{
		FString ReloadingString = "Reloading: ";
		ReloadingString.Append(FString::SanitizeFloat(GetRemainingReloadTime()));
		GEngine->AddOnScreenDebugMessage(850, GetRemainingReloadTime(), FColor::Yellow, ReloadingString);
	}
}

