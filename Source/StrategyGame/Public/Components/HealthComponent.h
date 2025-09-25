// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRATEGYGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:

	UPROPERTY(VisibleAnywhere, Category="Health")
	float Health;

	UPROPERTY(EditAnywhere, Category="Health", meta=(ClampMin=0.1f))
	float MaxHealth;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	float TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHealth() { return Health; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetMaxHealth() { return MaxHealth; }
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
