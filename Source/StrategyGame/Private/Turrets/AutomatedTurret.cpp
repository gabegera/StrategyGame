// Fill out your copyright notice in the Description page of Project Settings.


#include "Turrets/AutomatedTurret.h"

#include "Components/ShootingComponent.h"
#include "Components/SphereComponent.h"
#include "Enemies/EnemyShip.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AAutomatedTurret::AAutomatedTurret()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Turret Range");
	SphereComponent->SetupAttachment(SceneComponent);
	SphereComponent->SetCollisionProfileName("NoCollision");
	SphereComponent->SetSphereRadius(4096.0f);
}

// Called when the game starts or when spawned
void AAutomatedTurret::BeginPlay()
{
	Super::BeginPlay();
}

void AAutomatedTurret::ScanForEnemies()
{
	if (StructureState != EStructureState::ConstructionComplete) return;
	
	const FVector TraceStart = SphereComponent->GetComponentLocation();
	const FVector TraceEnd = TraceStart;
	const float Radius = SphereComponent->GetScaledSphereRadius();
	constexpr bool bTraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, Radius, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		bTraceComplex, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hits, true, FLinearColor::Yellow, FLinearColor::Green);

	AActor* ClosestEnemy = nullptr;
	float ClosestEnemyDistance = SphereComponent->GetScaledSphereRadius();
	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor() && Hit.GetActor()->IsA(AEnemyShip::StaticClass()))
		{
			if (GetDistanceTo(Hit.GetActor()) < ClosestEnemyDistance)
			{
				ClosestEnemy = Hit.GetActor();
				ClosestEnemyDistance = GetDistanceTo(Hit.GetActor());
			}
		}
	}
	TargetEnemy = ClosestEnemy;
}

void AAutomatedTurret::AimAtTarget(FVector TargetPos)
{
	const FRotator LookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPos);
	TurretMesh->SetWorldRotation(LookAtTargetRotation);
}

void AAutomatedTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ScanForEnemies();

	if (TargetEnemy)
	{
		AimAtTarget(TargetEnemy->GetComponentByClass<UStaticMeshComponent>()->GetComponentLocation());
		Fire();
	}
}
