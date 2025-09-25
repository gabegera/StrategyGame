// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableItem.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class STRATEGYGAME_API AMeleeWeapon : public AEquippableItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void UseItemPrimary() override;

	virtual void UseItemSecondary() override;

	UFUNCTION(BlueprintCallable, Category="Melee Weapon")
	void LightAttack();
	
	UFUNCTION(BlueprintCallable, Category="Melee Weapon")
	void HeavyAttack();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
