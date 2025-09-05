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
}

// Called when the game starts or when spawned
void AAutomatedTurret::BeginPlay()
{
	Super::BeginPlay();

	float TimerDuration = 0.2f;
	bool Loops = true;
	GetWorldTimerManager().SetTimer(ScanForEnemiesTimer, this, &ThisClass::ScanForEnemies, TimerDuration, Loops);
}

void AAutomatedTurret::ScanForEnemies()
{
	if (BuildableState != EBuildableState::ConstructionComplete) return;
	
	FVector TraceStart = SphereComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart;
	float Radius = SphereComponent->GetScaledSphereRadius();
	bool TraceComplex = false;
	TArray<AActor*> ActorsToIgnore;

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, Radius, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		TraceComplex, ActorsToIgnore, EDrawDebugTrace::ForDuration, Hits, true, FLinearColor::Yellow, FLinearColor::Green, 0.2f);

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
	FRotator LookAtTargetRotation = UKismetMathLibrary::FindLookAtRotation(TurretMesh->GetComponentLocation(), TargetPos);
	TurretMesh->SetWorldRotation(LookAtTargetRotation);
}

void AAutomatedTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetEnemy)
	{
		AimAtTarget(TargetEnemy->GetActorLocation());
		Fire();
	}
}
