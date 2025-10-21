// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/CitizenSpawn.h"

#include "Components/BillboardComponent.h"


// Sets default values
ACitizenSpawn::ACitizenSpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	SetRootComponent(CapsuleComponent);

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("Billboard");
	BillboardComponent->SetupAttachment(CapsuleComponent);
	BillboardComponent->bIsScreenSizeScaled = true;

	ConstructorHelpers::FObjectFinder<UTexture2D> BillboardFinder(TEXT("/Engine/EditorResources/S_Player.S_Player"));
	if (BillboardFinder.Succeeded())
	{
		BillboardComponent->SetSprite(BillboardFinder.Object);
	}
}

// Called when the game starts or when spawned
void ACitizenSpawn::BeginPlay()
{
	Super::BeginPlay();
	
}

