// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseStrategyWidget.generated.h"

class UUpgradeDataAsset;
class AStructure;
class AStrategyPlayerController;
class ARTSCamera;
class AStrategyGameState;
class ACityDefenseGameMode;

UCLASS()
class STRATEGYGAME_API UBaseStrategyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintGetter=GetStrategyPlayerController, Category="Strategy Widget")
	AStrategyPlayerController* StrategyPlayerController;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Structure Selected", Category="Strategy Widget")
	void OnStructureSelected(AStructure* SelectedStructure);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Structure DeSelected", Category="Strategy Widget")
	void OnStructureDeSelected();

public:

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	AStrategyPlayerController* GetStrategyPlayerController() const;
};
