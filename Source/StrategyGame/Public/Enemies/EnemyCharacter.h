// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StrategyGameCharacter.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class STRATEGYGAME_API AEnemyCharacter : public AStrategyGameCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Die() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
