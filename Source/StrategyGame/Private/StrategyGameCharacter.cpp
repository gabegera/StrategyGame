// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyGameCharacter.h"

#include "Components/ArrowComponent.h"
#include "EquippableItems/EquippableItem.h"


// Sets default values
AStrategyGameCharacter::AStrategyGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	CharacterInventoryComponent = CreateDefaultSubobject<UCharacterInventoryComponent>("Inventory");

	EquipmentPositionArrowComponent = CreateDefaultSubobject<UArrowComponent>("Dev Equipment Position");
	EquipmentPositionArrowComponent->SetupAttachment(GetMesh());
	EquipmentPositionArrowComponent->SetRelativeLocation(FVector(0.0f, 40.0f, 140.0f));
}

// Called when the game starts or when spawned
void AStrategyGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	OnItemEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemEquipped);
	OnItemUnEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemUnEquipped);
	OnItemPickedUpDelegate.AddUniqueDynamic(this, &ThisClass::OnItemPickedUp);
	OnItemDroppedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemDropped);
}

void AStrategyGameCharacter::OnItemEquipped(AEquippableItem* ItemEquipped)
{
	BP_OnItemEquipped(ItemEquipped);
}

void AStrategyGameCharacter::OnItemUnEquipped(AEquippableItem* ItemUnEquipped)
{
	BP_OnItemUnEquipped(ItemUnEquipped);
}

void AStrategyGameCharacter::OnItemPickedUp(AEquippableItem* ItemPickedUp)
{
	BP_OnItemPickedUp(ItemPickedUp);
}

void AStrategyGameCharacter::OnItemDropped(AEquippableItem* ItemDropped)
{
	BP_OnItemDropped(ItemDropped);
}

void AStrategyGameCharacter::OnDeath()
{
	ReactToDeath();
}

void AStrategyGameCharacter::UseEquippedItem()
{
	if (EquippedItem)
	{
		EquippedItem->UseItemPrimary();
	}
}

void AStrategyGameCharacter::ReleaseEquippedItem()
{
	if (EquippedItem)
	{
		EquippedItem->ReleaseItemPrimary();
	}
}

void AStrategyGameCharacter::UseEquippedItemSecondary()
{
	if (EquippedItem)
	{
		EquippedItem->UseItemSecondary();
	}
}

void AStrategyGameCharacter::ReleaseEquippedItemSecondary()
{
	if (EquippedItem)
	{
		EquippedItem->ReleaseItemSecondary();
	}
}

void AStrategyGameCharacter::ReloadEquippedItem()
{
	if (EquippedItem)
	{
		EquippedItem->Reload();
	}
}

void AStrategyGameCharacter::SpawnEquippable(TSubclassOf<AEquippableItem> EquippableClass)
{
	AEquippableItem* NewItem = GetWorld()->SpawnActor<AEquippableItem>(EquippableClass);
	PickupEquippable(NewItem);
}

void AStrategyGameCharacter::PickupEquippable(AEquippableItem* NewItem, bool EquipPickup)
{
	NewItem->OnPickedUpDelegate.Broadcast();
	
	NewItem->AttachToComponent(EquipmentPositionArrowComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewItem->SetActorRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CharacterInventoryComponent->AddEquippable(NewItem);
	
	OnItemPickedUpDelegate.Broadcast(NewItem);

	if (EquipPickup)
	{
		EquipItem(NewItem);
	}
}

void AStrategyGameCharacter::EquipItemBySlot(const EEquipmentSlot InSlot)
{
	if (!CharacterInventoryComponent) return;

	for (AEquippableItem* CarriedItem : CharacterInventoryComponent->GetCarriedEquipment())
	{
		if (CarriedItem->GetEquipmentSlot() == InSlot && CarriedItem != EquippedItem)
		{
			EquipItem(CarriedItem);
			return;
		}
	}
}

void AStrategyGameCharacter::EquipItemByClass(const TSubclassOf<AEquippableItem> InClass)
{
	if (!CharacterInventoryComponent) return;

	for (AEquippableItem* CarriedItem : CharacterInventoryComponent->GetCarriedEquipment())
	{
		if (CarriedItem->GetClass() == InClass && CarriedItem != EquippedItem)
		{
			EquipItem(CarriedItem);
			return;
		}
	}
}

void AStrategyGameCharacter::EquipItem(AEquippableItem* NewItem)
{
	if (EquippedItem)
	{
		HolsterEquippedItem();
	}

	if (!NewItem->IsAttachedTo(this))
	{
		PickupEquippable(NewItem);
	}

	EquippedItem = NewItem;
	
	EquippedItem->OnEquippedDelegate.Broadcast();
	OnItemEquippedDelegate.Broadcast(NewItem);
}

void AStrategyGameCharacter::HolsterEquippedItem()
{
	if (!EquippedItem) return;

	if (CharacterInventoryComponent)
	{
		CharacterInventoryComponent->AddEquippable(EquippedItem);
		
		EquippedItem->OnUnEquippedDelegate.Broadcast();
		OnItemUnEquippedDelegate.Broadcast(EquippedItem);
		
		EquippedItem = nullptr;
	}
	else
	{
		DropEquippedItem();
	}
}

void AStrategyGameCharacter::DropEquippedItem()
{
	if (!EquippedItem) return;

	CharacterInventoryComponent->RemoveEquippable(EquippedItem);
	EquippedItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	EquippedItem->OnDroppedDelegate.Broadcast();
	OnItemDroppedDelegate.Broadcast(EquippedItem);

	EquippedItem = nullptr;
}

float AStrategyGameCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(DamageAmount);

		if (HealthComponent->GetHealth() <= 0.0f)
		{
			Die();
		}
	}
	
	return HealthComponent->GetHealth();
}

void AStrategyGameCharacter::Die()
{
	if (bIsDead) return;

	bIsDead = true;	
	OnCharacterDiedDelegate.Broadcast(this);
	OnDeath();
}

// Called every frame
void AStrategyGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

