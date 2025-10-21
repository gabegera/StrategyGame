// Fill out your copyright notice in the Description page of Project Settings.


#include "EquippableItems/EquippableItem.h"

#include "Player/PlayerCharacter.h"


// Sets default values
AEquippableItem::AEquippableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Item Mesh");
	SetRootComponent(SkeletalMeshComponent);
	SkeletalMeshComponent->SetCollisionProfileName("PhysicsActorOverlapOnlyPawn");
	SkeletalMeshComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AEquippableItem::BeginPlay()
{
	Super::BeginPlay();
	
	OnEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnEquipped);
	OnUnEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnUnEquipped);
	OnPickedUpDelegate.AddUniqueDynamic(this, &ThisClass::OnPickedUp);
	OnDroppedDelegate.AddUniqueDynamic(this, &ThisClass::OnDropped);

	OnItemPrimaryUsedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemPrimaryUsed);
	OnItemPrimaryReleasedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemPrimaryReleased);
	OnItemSecondaryUsedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemSecondaryUsed);
	OnItemSecondaryReleasedDelegate.AddUniqueDynamic(this, &ThisClass::OnItemSecondaryReleased);
}

void AEquippableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AEquippableItem::OnEquipped()
{
	SetActorHiddenInGame(false);
	
	BP_OnEquipped();
}

void AEquippableItem::OnUnEquipped()
{
	SetActorHiddenInGame(true);
	
	BP_OnUnEquipped();
}

void AEquippableItem::OnPickedUp()
{
	SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    SkeletalMeshComponent->SetSimulatePhysics(false);
	
	BP_OnPickedUp();
}

void AEquippableItem::OnDropped()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SkeletalMeshComponent->SetSimulatePhysics(true);	
	
	BP_OnDropped();
}

void AEquippableItem::TryInteract(APlayerCharacter* InteractInstigator)
{
	InteractInstigator->PickupEquippable(this);
}

void AEquippableItem::UseItemPrimary()
{
	OnItemPrimaryUsedDelegate.Broadcast();
}

void AEquippableItem::ReleaseItemPrimary()
{
	OnItemPrimaryReleasedDelegate.Broadcast();
}

void AEquippableItem::UseItemSecondary()
{
	OnItemSecondaryUsedDelegate.Broadcast();
}

void AEquippableItem::ReleaseItemSecondary()
{
	OnItemSecondaryReleasedDelegate.Broadcast();
}

void AEquippableItem::Reload()
{
	
}

void AEquippableItem::CancelReload()
{
	
}

void AEquippableItem::ZoomPlayerCamera(float ZoomMultiplier)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		PlayerCharacter->GetFirstPersonCamera()->FieldOfView /= ZoomMultiplier;
	}
}

void AEquippableItem::ResetPlayerCameraZoom()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
	{
		PlayerCharacter->ResetFOV();
	}
}

// Called every frame
void AEquippableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

