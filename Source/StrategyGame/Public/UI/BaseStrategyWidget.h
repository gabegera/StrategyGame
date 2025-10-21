// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseStrategyWidget.generated.h"

class UUpgradeDataAsset;
class AStructure;
class ARTSPlayerController;
class ARTSCamera;
class AStrategyGameState;
class ACityDefenseGameMode;

UCLASS()
class STRATEGYGAME_API UBaseStrategyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintGetter=GetRTSPlayerController, Category="Strategy Widget")
	ARTSPlayerController* RTSPlayerController;

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	ARTSPlayerController* GetRTSPlayerController() const { return RTSPlayerController; }
};
