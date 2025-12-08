// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StructureComponent.generated.h"

enum class ECitizenType : uint8;
class ACitizen;
class AStructure;

// The Base Class for a component that is meant to be attached to a structure class.
// The component activates only when the structure it's attached to has finished construction.
UCLASS(Abstract, Within=Structure)
class STRATEGYGAME_API UStructureComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStructureComponent();

protected:

	// The structure that owns this component.
	UPROPERTY()
	AStructure* OwningStructure;

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION()
	virtual void OnStructureBuilt(AStructure* BuiltStructure);

	UFUNCTION()
	virtual void OnStructureDestroyed(AStructure* DestroyedStructure);

	UFUNCTION()
	virtual void OnTimePassed(float HoursPassed);

};
