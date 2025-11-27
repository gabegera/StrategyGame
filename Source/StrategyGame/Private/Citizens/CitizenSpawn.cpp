// Fill out your copyright notice in the Description page of Project Settings.


#include "Citizens/CitizenSpawn.h"

#include "StrategyEnums.h"
#include "Components/BillboardComponent.h"
#include "Citizens/Citizen.h"

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

	ConstructorHelpers::FClassFinder<ACitizen> CitizenClassFinder(TEXT("/Game/Blueprints/AI/Citizens/BP_Citizen.BP_Citizen_C"));
	if (CitizenClassFinder.Succeeded())
	{
		CitizenClassToSpawn = CitizenClassFinder.Class;
	}
	else
	{
		CitizenClassToSpawn = ACitizen::StaticClass();
	}
}

// Called when the game starts or when spawned
void ACitizenSpawn::BeginPlay()
{
	Super::BeginPlay();
}

void ACitizenSpawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void ACitizenSpawn::SpawnCitizensInGrid(const int32 NumOfWorkers, const int32 NumOfScientists)
{
	float CitizenBoundsSize = CitizenClassToSpawn.GetDefaultObject()->GetCapsuleComponent()->GetLocalBounds().BoxExtent.GetAbsMax();
	float DistanceBetweenCitizens = CitizenBoundsSize + CitizenBoundsSize / 2;
	
	const int32 TotalNumberOfCitizens = NumOfWorkers + NumOfScientists;
	int32 CitizensPerRow = FMath::CeilToInt32(FMath::Sqrt(static_cast<float>(TotalNumberOfCitizens)));
	
	FVector SpawnPosition = GetActorLocation();
	SpawnPosition -= GetActorRightVector() / 2 * DistanceBetweenCitizens * CitizensPerRow;
	SpawnPosition += GetActorForwardVector() / 2 * DistanceBetweenCitizens * CitizensPerRow;
	SpawnPosition += FVector::UpVector * 2000.0f;

	FVector StartingSpawnPos = SpawnPosition;
	
	int32 WorkerCounter = 0;
	for (int32 i = 1; i < TotalNumberOfCitizens + 1; i++)
	{
		ACitizen* CitizenToSpawn = GetWorld()->SpawnActorDeferred<ACitizen>(CitizenClassToSpawn, FTransform());
		if (WorkerCounter == NumOfWorkers)
		{
			CitizenToSpawn->SetCitizenType(ECitizenType::Scientist);
		}
		else
		{
			CitizenToSpawn->SetCitizenType(ECitizenType::Worker);
			WorkerCounter++;
		}

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, SpawnPosition, SpawnPosition - FVector::UpVector * 10000.0f, ECC_Visibility);
		FTransform SpawnTransform = FTransform(HitResult.bBlockingHit ? HitResult.ImpactPoint + FVector::UpVector * CitizenBoundsSize : SpawnPosition);
		CitizenToSpawn->FinishSpawning(SpawnTransform);

		if (i % CitizensPerRow == 0)
		{
			SpawnPosition = StartingSpawnPos - GetActorForwardVector() * FMath::Floor(i / CitizensPerRow) * DistanceBetweenCitizens;
		}
		else
		{
			SpawnPosition += GetActorRightVector() * DistanceBetweenCitizens;
		}
	}
}
