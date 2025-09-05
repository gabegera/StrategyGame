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

	UPROPERTY()
	float Damage;

	UPROPERTY()
	float KnockbackForceMultiplier;

	UPROPERTY()
	FTimerHandle ProjectileDestructionTimer;

	UPROPERTY()
	FVector PreviousLocation = FVector::ZeroVector;

	UPROPERTY()
	AActor* Spawner = nullptr;
	
	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Checks if the projectile has hit anything between its previous location and where it is currently.
	UFUNCTION(BlueprintCallable)
	void CheckCollision();

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

	void AddActorToIgnore(AActor* Actor) { ActorsToIgnore.Add(Actor); }
	void AppendActorsToIgnore(TArray<AActor*> Actors) { ActorsToIgnore.Append(Actors); }

	UFUNCTION(BlueprintCallable)
	void SetSpawner(AActor* NewSpawner) { Spawner = NewSpawner; }

	UFUNCTION(BlueprintCallable)
	float SetDamage(const float NewDamage) { return Damage = NewDamage; }

	UFUNCTION(BlueprintCallable)
	float SetKnockbackForceMultiplier(const float NewForce) { return KnockbackForceMultiplier = NewForce; }

};
