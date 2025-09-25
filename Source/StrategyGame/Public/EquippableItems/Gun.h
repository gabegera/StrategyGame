// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableItem.h"
#include "Components/ShootingComponent.h"
#include "Gun.generated.h"

class UArrowComponent;

UCLASS()
class STRATEGYGAME_API AGun : public AEquippableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

protected:

	UPROPERTY(EditAnywhere, Category="Equippable Item|Components")
	UArrowComponent* MuzzleArrowComponent;

	UPROPERTY(EditAnywhere, Category="Equippable Item|Components")
	UShootingComponent* ShootingComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnEquipped() override;

	virtual void OnUnEquipped() override;

	virtual void OnPickedUp() override;

	virtual void OnDropped() override;

public:

	virtual void UseItemPrimary() override;

	virtual void ReleaseItemPrimary() override;

	virtual void UseItemSecondary() override;

	virtual void ReleaseItemSecondary() override;

	virtual void Reload() override;

	virtual void CancelReload() override;

	UFUNCTION(BlueprintCallable, Category="Gun")
	virtual void Fire();

	UFUNCTION(BlueprintCallable, Category="Gun")
	virtual void StartAiming();

	UFUNCTION(BlueprintCallable, Category="Gun")
	virtual void StopAiming();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
