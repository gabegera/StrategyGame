// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Components/HealthComponent.h"
#include "Destructible.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestructibleDestroyedDelegate, TSubclassOf<ADestructible>, DestroyedDestructibleClass);

UCLASS()
class STRATEGYGAME_API ADestructible : public ACustomActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructible();

protected:

	UPROPERTY(EditAnywhere, Category="Destructible|Components")
	UHealthComponent* HealthComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnDestructibleDestroyedDelegate OnDestructibleDestroyedDelegate;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
