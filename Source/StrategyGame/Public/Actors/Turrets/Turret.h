// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectile.h"
#include "Actors/CustomActor.h"
#include "Turret.generated.h"

class UArrowComponent;

UCLASS()
class STRATEGYGAME_API ATurret : public ACustomActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

protected:

	// ------ COMPONENTS ------

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent = nullptr;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow = nullptr;

	// If melee weapon, controls how much damage will be done when the weapon is thrown.
	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 10.0f;
	
	// Multiplied by the Projectile Velocity and damage to knockback collided actors.
	UPROPERTY(EditAnywhere, Category="Weapon")
	float KnockbackForceMultiplier = 1.0f;
	
	// The Projectile that this weapon fires.
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AProjectile> Projectile;
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpeed = 30000.0f;

	// How many projectiles are spawned when the weapon is fired.
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(UIMin=1, ClampMin=1, EditConditionHides))
	int32 ProjectileCount = 1;
	
	// The weapons fire rate in Rounds Per Minute.
	UPROPERTY(EditAnywhere, Category="Weapon")
	float FireRateRpm;
	UPROPERTY() FTimerHandle FireRateTimer;
	
	// ------ AMMO ------

	UPROPERTY(EditAnywhere, Category="Ammo")
	bool InfiniteAmmo = false;

	// How much ammo is available for a reload.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 Ammo;

	// How much ammo can be carried at one time.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 MaxAmmo;

	// How much ammo the magazine can hold.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 MagazineCapacity;

	// How much ammo is currently in the magazine.
	UPROPERTY()
	int32 AmmoInMagazine;

	// How long it takes to reload the weapon.
	UPROPERTY(EditAnywhere, Category="Ammo")
	float ReloadTime = 1.0f;
	
	UPROPERTY()
	FTimerHandle ReloadTimer;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Fire();

	virtual void Reload();

	void ShootProjectile(FVector Target);
	void ShootProjectileForward();

	UFUNCTION(BlueprintCallable)
	float RPMToSeconds(float FireRate) { return 1 / (FireRate / 60); }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UArrowComponent* GetArrowComponent() const { return Arrow; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsReloading() { return GetWorldTimerManager().IsTimerActive(ReloadTimer); }
};
