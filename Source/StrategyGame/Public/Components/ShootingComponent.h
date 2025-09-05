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
	
	UPROPERTY(EditAnywhere, Category="Shooting")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, Category="Shooting")
	float KnockbackForceMultiplier = 1.0f;

	// If the projectile is null, this weapon will fire Hitscan / LineTrace.
	UPROPERTY(EditAnywhere, Category="Shooting")
	TSubclassOf<AProjectile> Projectile;
	
	UPROPERTY(EditAnywhere, Category="Shooting")
	float ProjectileSpeed = 30000.0f;

	// How many projectiles are spawned when the weapon is fired. Useful if the weapon is a shotgun.
	UPROPERTY(EditAnywhere, Category="Shooting", meta=(UIMin=1, ClampMin=1, EditConditionHides))
	int32 ProjectileCount = 1;
	
	UPROPERTY(EditAnywhere, Category="Shooting")
	float FireRateRoundsPerMinute;
	
	UPROPERTY()
	FTimerHandle FireRateTimer;

	UPROPERTY(EditAnywhere, Category="Shooting")
	bool BottomlessClip = false;
	
	UPROPERTY(EditAnywhere, Category="Shooting", meta=(EditCondition="!BottomlessClip"))
	int32 MagazineCapacity = 100;
	
	UPROPERTY(VisibleAnywhere, Category="Shooting")
	int32 AmmoInMagazine = 0;
	
	UPROPERTY(EditAnywhere, Category="Shooting")
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

	UFUNCTION(BlueprintCallable, Category="Reload")
	void StartReload();

protected:

	void CompleteReload();

public:
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float RoundsPerMinuteToRoundsPerSecond(float FireRate) { return 1 / (FireRate / 60); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsReloading() { return GetWorld()->GetTimerManager().IsTimerActive(ReloadTimer); }
};
