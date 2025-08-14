// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile.h"

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
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ProjectileDestructionTimer, this, &AProjectile::DestroyProjectile, ProjectileLifetime, false);

	if (Spawner) AddActorToIgnore(Spawner);
}

void AProjectile::CheckCollision()
{
	if (PreviousLocation == FVector::ZeroVector) PreviousLocation = GetActorLocation();
	
	FHitResult Hit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PreviousLocation, GetActorLocation(), Sphere->GetScaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	PreviousLocation = GetActorLocation();

	if (!Hit.GetActor()) return;
		
	Hit.GetActor()->TakeDamage(Damage, FPointDamageEvent(), GetInstigatorController(), GetInstigator());

	GEngine->AddOnScreenDebugMessage(20, 3.0f, FColor::Green, Hit.GetActor()->GetName());

	if (UStaticMeshComponent* SMComp = Hit.GetActor()->GetComponentByClass<UStaticMeshComponent>())
	{
		if (SMComp->IsSimulatingPhysics()) SMComp->AddImpulseAtLocation(GetVelocity() * KnockbackForceMultiplier * Damage, Hit.ImpactPoint);
	}
	
	DestroyProjectile();
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCollision();
}

