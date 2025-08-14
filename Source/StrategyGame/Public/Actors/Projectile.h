// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "Projectile.generated.h"

UCLASS()
class STRATEGYGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	// The Actor that spawned this projectile.
	UPROPERTY() AActor* Spawner;

	UPROPERTY() float Damage;

	UPROPERTY() float KnockbackForceMultiplier;

	UPROPERTY(EditAnywhere)
	float ProjectileLifetime = 8.0f;
	FTimerHandle ProjectileDestructionTimer;

	UPROPERTY() FVector PreviousLocation = FVector::ZeroVector;

	// Array of Actors for the collision to ignore.
	UPROPERTY() TArray<AActor*> ActorsToIgnore;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Checks if the projectile has hit anything between its previous location and where it is currently.
	UFUNCTION(BlueprintCallable)
	void CheckCollision();

	UFUNCTION(BlueprintCallable)
	void DestroyProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetMesh() const { return Mesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USphereComponent* GetSphere() const { return Sphere; }

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	void SetSpawner(AActor* Actor) { Spawner = Actor; }

	void AddActorToIgnore(AActor* Actor) { ActorsToIgnore.Add(Actor); }
	void AppendActorsToIgnore(TArray<AActor*> Actors) { ActorsToIgnore.Append(Actors); }

	UFUNCTION(BlueprintCallable)
	float SetDamage(const float NewDamage) { return Damage = NewDamage; }

	UFUNCTION(BlueprintCallable)
	float SetKnockbackForceMultiplier(const float NewForce) { return KnockbackForceMultiplier = NewForce; }

};
