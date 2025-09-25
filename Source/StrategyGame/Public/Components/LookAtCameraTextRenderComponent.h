// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "LookAtCameraTextRenderComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRATEGYGAME_API ULookAtCameraTextRenderComponent : public UTextRenderComponent
{
	GENERATED_BODY()

public:

	ULookAtCameraTextRenderComponent();

protected:

	

public:

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
