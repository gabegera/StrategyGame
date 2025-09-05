// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Building/Buildable.h"
#include "Turret.generated.h"

class UArrowComponent;
class UShootingComponent;

UCLASS()
class STRATEGYGAME_API ATurret : public ABuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

protected:

	UPROPERTY(EditAnywhere, Category="Turret")
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, Category="Turret")
	UArrowComponent* ArrowComponent;

	UPROPERTY(EditAnywhere, Category="Turret")
	UShootingComponent* ShootingComponent;

	UPROPERTY(VisibleAnywhere, Category="Turret|Muzzles")
	TArray<UArrowComponent*> MuzzleLocations;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Fire();

	virtual void Reload();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Turret|Getters")
	UShootingComponent* GetShootingComponent() { return ShootingComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Turret|Getters")
	TArray<UArrowComponent*> GetMuzzleLocations() { return MuzzleLocations; }

	
};
