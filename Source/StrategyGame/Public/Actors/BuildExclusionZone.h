// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CustomActor.h"
#include "BuildExclusionZone.generated.h"

UCLASS()
class STRATEGYGAME_API ABuildExclusionZone : public ACustomActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildExclusionZone();

protected:

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
