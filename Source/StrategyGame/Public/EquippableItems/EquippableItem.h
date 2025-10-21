// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Interfaces/InteractionInterface.h"
#include "EquippableItem.generated.h"

UENUM(BlueprintType, DisplayName="Equipment Slot")
enum class EEquipmentSlot : uint8
{
	Slot1 = 0 UMETA(DisplayName="Slot 1"),
	Slot2 = 1 UMETA(DisplayName="Slot 2"),
	Slot3 = 2 UMETA(DisplayName="Slot 3"),
	Slot4 = 3 UMETA(DisplayName="Slot 4"),
	Slot5 = 4 UMETA(DisplayName="Slot 5"),
	NoSlot = 5 UMETA(DisplayName="No Slot") // Without a slot the item will need to be equipped by class.
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquippedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnEquippedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPickedUpDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDroppedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemPrimaryUsedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemPrimaryReleasedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemSecondaryUsedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemSecondaryReleasedDelegate);

UCLASS()
class STRATEGYGAME_API AEquippableItem : public ACustomActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquippableItem();

protected:

	UPROPERTY(EditAnywhere, Category="Equippable Item|Components")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere, Category="Equippable Item")
	EEquipmentSlot EquipmentSlot = EEquipmentSlot::Slot1;

	UPROPERTY(EditAnywhere, Category="Equippable Item")
	FString DisplayName = "Unnamed Equippable";

	UPROPERTY(EditAnywhere, Category="Equippable Item|Animations")
	UAnimationAsset* ItemAnimation;

	UPROPERTY(EditAnywhere, Category="Equippable Item|Animations")
	UAnimationAsset* FPPlayerItemAnimation;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	virtual void OnEquipped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnEquipped")
	void BP_OnEquipped();

	UFUNCTION()
	virtual void OnUnEquipped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnUnEquipped")
	void BP_OnUnEquipped();

	UFUNCTION()
	virtual void OnPickedUp();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnPickedUp")
	void BP_OnPickedUp();

	UFUNCTION()
	virtual void OnDropped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, DisplayName="OnDropped")
	void BP_OnDropped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemPrimaryUsed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemPrimaryReleased();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemSecondaryUsed();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemSecondaryReleased();

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnEquippedDelegate OnEquippedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnUnEquippedDelegate OnUnEquippedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnPickedUpDelegate OnPickedUpDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnDroppedDelegate OnDroppedDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemPrimaryUsedDelegate OnItemPrimaryUsedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemPrimaryReleasedDelegate OnItemPrimaryReleasedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemSecondaryUsedDelegate OnItemSecondaryUsedDelegate;
	
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemSecondaryReleasedDelegate OnItemSecondaryReleasedDelegate;

	virtual void TryInteract(APlayerCharacter* InteractInstigator) override;

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void UseItemPrimary();

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void ReleaseItemPrimary();

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void UseItemSecondary();

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void ReleaseItemSecondary();

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	virtual void CancelReload();

	// Attempts to find the player camera and zooms it in by the input multiplier.
	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	void ZoomPlayerCamera(float ZoomMultiplier = 1.0f);

	// Resets the player camera zoom to default value.
	UFUNCTION(BlueprintCallable, Category="Equippable Item")
	void ResetPlayerCameraZoom();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EEquipmentSlot GetEquipmentSlot() const { return EquipmentSlot; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAnimationAsset* GetFPPlayerItemAnimation() { return FPPlayerItemAnimation; }
};
