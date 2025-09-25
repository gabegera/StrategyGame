// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CharacterInventoryComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/Character.h"
#include "StrategyGameCharacter.generated.h"

enum class EEquipmentSlot : uint8;
class AEquippableItem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquippedDelegate, AEquippableItem*, EquippedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemUnEquippedDelegate, AEquippableItem*, UnEquippedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemPickedUpDelegate, AEquippableItem*, PickedUpItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemDroppedDelegate, AEquippableItem*, DroppedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, AStrategyGameCharacter*, DeadCharacter);

UCLASS()
class STRATEGYGAME_API AStrategyGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStrategyGameCharacter();

protected:

	UPROPERTY(EditAnywhere, Category="Strategy Game Character|Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, Category="Strategy Game Character|Components")
	UCharacterInventoryComponent* CharacterInventoryComponent;

	UPROPERTY(EditAnywhere, Category="Strategy Game Character|Components")
	UArrowComponent* DevEquipmentPositionArrow;
	
	UPROPERTY(VisibleAnywhere)
	AEquippableItem* EquippedItem = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Strategy Game Character")
	bool bIsDead = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnItemEquipped(AEquippableItem* ItemEquipped);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnItemEquipped")
	void BP_OnItemEquipped(AEquippableItem* ItemEquipped);

	UFUNCTION()
	virtual void OnItemUnEquipped(AEquippableItem* ItemUnEquipped);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnItemUnEquipped")
	void BP_OnItemUnEquipped(AEquippableItem* ItemUnEquipped);

	UFUNCTION()
	virtual void OnItemPickedUp(AEquippableItem* ItemPickedUp);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnItemPickedUp")
	void BP_OnItemPickedUp(AEquippableItem* ItemPickedUp);

	UFUNCTION()
	virtual void OnItemDropped(AEquippableItem* ItemDropped);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnItemDropped")
	void BP_OnItemDropped(AEquippableItem* ItemDropped);

	UFUNCTION()
	virtual void OnDeath();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnDeath")
	void BP_OnDeath();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemEquippedDelegate OnItemEquippedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemUnEquippedDelegate OnItemUnEquippedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemPickedUpDelegate OnItemPickedUpDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemDroppedDelegate OnItemDroppedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCharacterDied OnCharacterDiedDelegate;

	UFUNCTION(BlueprintCallable)
	virtual void UseEquippedItem();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseEquippedItem();

	UFUNCTION(BlueprintCallable)
	virtual void UseEquippedItemSecondary();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseEquippedItemSecondary();

	UFUNCTION(BlueprintCallable)
	virtual void ReloadEquippedItem();

	UFUNCTION(BlueprintCallable)
	void PickupEquippable(AEquippableItem* NewItem, bool EquipPickup = false);

	// Equips the item that is currently occupying the desired slot.
	UFUNCTION(BlueprintCallable)
	virtual void EquipItemBySlot(const EEquipmentSlot TargetSlot);

	// Equips a new item by a pointer to the new item.
	UFUNCTION(BlueprintCallable)
	virtual void EquipItem(AEquippableItem* NewItem);

	// Holsters the currently equipped item.
	UFUNCTION(BlueprintCallable)
	virtual void HolsterEquippedItem();

	// Drops the currently equipped item.
	UFUNCTION(BlueprintCallable)
	virtual void DropEquippedItem();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintCallable)
	virtual void Die();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Strategy Game Character")
	UHealthComponent* GetHealthComponent() { return HealthComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Strategy Game Character")
	UCharacterInventoryComponent* GetCharacterInventoryComponent() { return CharacterInventoryComponent; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Strategy Game Character")
	bool IsDead() const { return bIsDead; }
};
