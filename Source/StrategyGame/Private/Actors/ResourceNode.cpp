// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ResourceNode.h"


// Sets default values
AResourceNode::AResourceNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);

	SphereTrigger = CreateDefaultSubobject<USphereComponent>("Sphere Trigger");
	SphereTrigger->SetupAttachment(StaticMesh);
	SphereTrigger->SetCollisionProfileName("Trigger");
	SphereTrigger->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AResourceNode::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AResourceNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

