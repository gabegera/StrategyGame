// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseStrategyWidget.generated.h"

class AStructure;
class ASkyscraper;
class ASkyscraperModule;
class ABuildable;
class ARTSPlayerController;
class ARTSCamera;
class AStrategyGameState;
class AStrategyGameModeBase;

UCLASS()
class STRATEGYGAME_API UBaseStrategyWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	// Do not call directly, use their getter functions.
	UPROPERTY(BlueprintGetter=GetStrategyGameState, Category="Strategy Widget")
	AStrategyGameState* StrategyGameState;

	// Do not call directly, use their getter functions.
	UPROPERTY(BlueprintGetter=GetStrategyGameMode, Category="Strategy Widget")
	AStrategyGameModeBase* StrategyGameMode;

	UPROPERTY(BlueprintGetter=GetRTSPlayerController, Category="Strategy Widget")
	ARTSPlayerController* RTSPlayerController;

	UPROPERTY(BlueprintGetter=GetRTSCamera, Category="Strategy Widget")
	ARTSCamera* RTSCamera;

	virtual void NativeConstruct() override;

	
	UFUNCTION()
	void OnControllerPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnControllerPawnChanged")
	void BP_OnControllerPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UFUNCTION()
	void OnBuildableSelected(ABuildable* SelectedBuildable);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnBuildableSelected")
	void BP_OnBuildableSelected(ABuildable* SelectedBuildable);

	UFUNCTION()
	void OnBuildableDeSelected();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnBuildableDeSelected")
	void BP_OnBuildableDeSelected();

	UFUNCTION()
	void OnResourcesChanged();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnResourcesChanged")
	void BP_OnResourcesChanged();

	UFUNCTION()
	void OnPopulationChanged();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnPopulationChanged")
	void BP_OnPopulationChanged();

	UFUNCTION()
	void OnAssignedWorkersChanged();

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnAssignedWorkersChanged")
	void BP_OnAssignedWorkersChanged();

	UFUNCTION()
	void OnStructureBuilt(AStructure* BuiltStructure);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnStructureBuilt")
	void BP_OnStructureBuilt(AStructure* BuiltStructure);

	UFUNCTION()
	void OnSkyscraperModuleAdded(ASkyscraper* Skyscraper, ASkyscraperModule* AddedModule);

	UFUNCTION(BlueprintImplementableEvent, DisplayName="OnSkyscraperModuleAdded")
	void BP_OnSkyscraperModuleAdded(ASkyscraper* Skyscraper, ASkyscraperModule* AddedModule);


public:

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	AStrategyGameState* GetStrategyGameState();

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	AStrategyGameModeBase* GetStrategyGameMode();

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	ARTSPlayerController* GetRTSPlayerController();

	UFUNCTION(BlueprintGetter, Category="Strategy Widget|Getters")
	ARTSCamera* GetRTSCamera();
};
