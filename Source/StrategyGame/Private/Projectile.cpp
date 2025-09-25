// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionProfileName("NoCollision");

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);
	Mesh->SetCollisionProfileName("NoCollision");

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->bRotationFollowsVelocity = true;

	InitialLifeSpan = 5.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::CheckCollision()
{
	if (PreviousLocation == FVector::ZeroVector) PreviousLocation = GetActorLocation();

	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PreviousLocation, GetActorLocation(), Sphere->GetScaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_Camera), false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, Hit, true);

	PreviousLocation = GetActorLocation();

	if (!Hit.GetActor()) return;

	FString ProjectileHitString = "Projectile Hit: ";
	ProjectileHitString.Append(Hit.GetActor()->GetName());
	GEngine->AddOnScreenDebugMessage(20, 3.0f, FColor::Red, ProjectileHitString);

	if (UStaticMeshComponent* HitStaticMesh = Hit.GetActor()->GetComponentByClass<UStaticMeshComponent>())
	{
		if (HitStaticMesh->IsSimulatingPhysics())
		{
			HitStaticMesh->AddImpulseAtLocation(GetVelocity() * KnockbackForceMultiplier, Hit.ImpactPoint);
		}
	}

	if (USkeletalMeshComponent* HitSkeletalMesh = Hit.GetActor()->GetComponentByClass<USkeletalMeshComponent>())
	{
		if (HitSkeletalMesh->IsSimulatingPhysics())
		{
			HitSkeletalMesh->AddImpulseAtLocation(GetVelocity() * KnockbackForceMultiplier, Hit.ImpactPoint);
		}
	}

	Hit.GetActor()->TakeDamage(Damage, FPointDamageEvent(), GetInstigatorController(), GetInstigator());
	
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCollision();
}

