// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootingComponent.generated.h"

class AProjectile;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRATEGYGAME_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShootingComponent();

protected:
	
	UPROPERTY(EditAnywhere, Category="Shooting|Damage")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Damage")
	float KnockbackForceMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, Category="Shooting|Aiming")
	bool bIsAiming = false;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Aiming")
	float AimingZoomMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Category="Shooting|Spread")
	float MaxHipfireSpreadDegrees = 0.0f;

	UPROPERTY(EditAnywhere, Category="Shooting|Spread")
	float MaxAimingSpreadDegrees = 0.0f;

	// If the projectile is null, this weapon will fire Hitscan / LineTrace.
	UPROPERTY(EditAnywhere, Category="Shooting|Projectiles")
	TSubclassOf<AProjectile> Projectile;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Projectiles")
	float ProjectileSpeed = 30000.0f;

	// How many projectiles are spawned when the weapon is fired. Useful if the weapon is a shotgun.
	UPROPERTY(EditAnywhere, Category="Shooting|Projectiles", meta=(UIMin=1, ClampMin=1))
	int32 ProjectileCount = 1;

	UPROPERTY(EditAnywhere, Category="Shooting|Fire Rate")
	bool bIsSemiAuto = false;

	// Used to control the fire rate of semi-auto weapons.
	UPROPERTY(VisibleAnywhere, Category="Shooting|Fire Rate")
	bool bIsTriggerPulled = false;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Fire Rate", meta=(EditCondition="!bIsSemiAuto"))
	float FireRateRoundsPerMinute;
	
	UPROPERTY()
	FTimerHandle FireRateTimer;

	// When true the weapon doesn't cost any ammo to fire.
	UPROPERTY(EditAnywhere, Category="Shooting|Ammo")
	bool bInfiniteAmmo = false;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Ammo", meta=(EditCondition="!bInfiniteAmmo"))
	int32 MagazineCapacity = 100;
	
	UPROPERTY(VisibleAnywhere, Category="Shooting|Ammo")
	int32 AmmoInMagazine = 0;
	
	UPROPERTY(EditAnywhere, Category="Shooting|Ammo")
	float ReloadTimeInSeconds = 1.0f;
	
	UPROPERTY()
	FTimerHandle ReloadTimer;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void ShootProjectile(FVector ShotStart, FVector ShotTarget);

	void ShootHitscan(FVector ShotStart, FVector ShotTarget);

public:

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void Shoot(FVector ShotStart, FVector ShotTarget, bool ShouldStartFireRateTimer);

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void StartAiming();

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void StopAiming();

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void StartReload();

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void CancelReload();

	UFUNCTION(BlueprintCallable, Category="Shooting")
	void ResetFireRate();

protected:

	void CompleteReload();

public:
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float RoundsPerMinuteToRoundsPerSecond(float FireRate) { return 1 / (FireRate / 60); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsReloading() { return GetWorld()->GetTimerManager().IsTimerActive(ReloadTimer); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAiming() { return bIsAiming; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRemainingReloadTime() { return GetWorld()->GetTimerManager().GetTimerRemaining(ReloadTimer); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAimingZoomMultiplier() { return AimingZoomMultiplier; }
};
