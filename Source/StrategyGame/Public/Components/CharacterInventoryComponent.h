// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterInventoryComponent.generated.h"

class AEquippableItem;
class AWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRATEGYGAME_API UCharacterInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterInventoryComponent();

protected:

	UPROPERTY(VisibleAnywhere, Category="Character Inventory")
	TArray<AEquippableItem*> CarriedEquipment;

	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable, Category="Character Inventory")
	TArray<AEquippableItem*>& GetCarriedEquipment() { return CarriedEquipment; }

	UFUNCTION(BlueprintCallable, Category="Character Inventory")
	void AddEquippable(AEquippableItem* NewEquippable);

	UFUNCTION(BlueprintCallable, Category="Character Inventory")
	void RemoveEquippable(AEquippableItem* EquippableToRemove);
};
