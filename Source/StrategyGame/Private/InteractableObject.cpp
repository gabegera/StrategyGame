// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableObject.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractableObject::OnInteract(APlayerCharacter* InteractInstigator)
{
	GEngine->AddOnScreenDebugMessage(20, 3.0f, FColor::Yellow, GetName() + " Interacted With");
}

bool AInteractableObject::Interact(APlayerCharacter* InteractInstigator)
{
	OnInteract(InteractInstigator);
	BP_OnInteract(InteractInstigator);
	
	return IInteractionInterface::Interact(InteractInstigator);
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}