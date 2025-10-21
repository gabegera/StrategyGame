// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "CitizenSpawn.generated.h"

UCLASS(NotBlueprintable)
class STRATEGYGAME_API ACitizenSpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACitizenSpawn();

protected:

	UPROPERTY()
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	UBillboardComponent* BillboardComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
