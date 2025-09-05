// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Turret.h"
#include "AutomatedTurret.generated.h"

class AEnemyShip;
class USphereComponent;

UCLASS()
class STRATEGYGAME_API AAutomatedTurret : public ATurret
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAutomatedTurret();

protected:

	UPROPERTY(EditAnywhere, Category="Automated Turret")
	USphereComponent* SphereComponent;

	UPROPERTY()
	FTimerHandle ScanForEnemiesTimer;

	UPROPERTY(VisibleAnywhere, Category="Automated Turret")
	AActor* TargetEnemy = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category="AutomatedTurret")
	void ScanForEnemies();

	UFUNCTION(BlueprintCallable, Category="AutomatedTurret")
	void AimAtTarget(FVector TargetPos);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
