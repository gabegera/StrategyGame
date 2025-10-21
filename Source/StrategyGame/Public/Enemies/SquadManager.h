// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SquadManager.generated.h"

class AStrategyGameCharacter;

/**
 * In charge of managing the maximum number of enemies in a squad and giving all of them orders.
 */
UCLASS()
class STRATEGYGAME_API USquadManager : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category="Squad")
	uint16 MaxSquadCapacity = 6;

	// What class of character to spawn squad members.
	UPROPERTY(EditAnywhere, Category="Squad")
	TSubclassOf<AStrategyGameCharacter> SquadMemberClass;

	UPROPERTY(VisibleAnywhere, Category="Squad")
	TSet<AStrategyGameCharacter*> SquadMembers;
	
	void SpawnSquadMembers();

	void OnSquadMemberTookDamage();
	
	void OnSquadMemberDied();

public:

	UFUNCTION(BlueprintCallable, Category="Squad")
	void SetAttackTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="Squad")
	void SetTargetPosition(FVector InPosition);
};
