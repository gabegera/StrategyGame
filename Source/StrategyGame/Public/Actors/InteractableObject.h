// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomActor.h"
#include "Interfaces/InteractionInterface.h"
#include "InteractableObject.generated.h"

class AStrategyGameState;

UCLASS()
class STRATEGYGAME_API AInteractableObject : public ACustomActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	virtual bool Interact_Implementation(APlayerCharacter* InteractInstigator);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
