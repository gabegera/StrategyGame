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

	OnEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnEquipped);
	OnUnEquippedDelegate.AddUniqueDynamic(this, &ThisClass::OnUnEquipped);
	OnPickedUpDelegate.AddUniqueDynamic(this, &ThisClass::OnPickedUp);
	OnDroppedDelegate.AddUniqueDynamic(this, &ThisClass::OnDropped);
}

// Called when the game starts or when spawned
void AEquippableItem::BeginPlay()
{
	Super::BeginPlay();
}

void AEquippableItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AEquippableItem::OnEquipped()
{
	BP_OnEquipped();

	SetActorHiddenInGame(false);
}

void AEquippableItem::OnUnEquipped()
{
	BP_OnUnEquipped();

	SetActorHiddenInGame(true);
}

void AEquippableItem::OnPickedUp()
{
	BP_OnPickedUp();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SkeletalMeshComponent->SetSimulatePhysics(false);
}

void AEquippableItem::OnDropped()
{
	BP_OnDropped();

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SkeletalMeshComponent->SetSimulatePhysics(true);
}

bool AEquippableItem::Interact(APlayerCharacter* InteractInstigator)
{
	InteractInstigator->PickupEquippable(this);
	
	return true;
}

void AEquippableItem::UseItemPrimary()
{
	
}

void AEquippableItem::ReleaseItemPrimary()
{
	
}

void AEquippableItem::UseItemSecondary()
{
	
}

void AEquippableItem::ReleaseItemSecondary()
{
	
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

