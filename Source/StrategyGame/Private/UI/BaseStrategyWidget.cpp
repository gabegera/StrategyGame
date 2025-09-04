// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/BaseStrategyWidget.h"

#include "Game/StrategyGameState.h"
#include "Game/StrategyGameModeBase.h"
#include "Player/RTSPlayerController.h"
#include "Player/RTSCamera.h"

void UBaseStrategyWidget::NativeConstruct()
{
	if (GetRTSPlayerController()) GetRTSPlayerController()->OnPossessedPawnChanged.AddUniqueDynamic(this, &ThisClass::OnControllerPawnChanged);
	if (GetStrategyGameState())
	{
		GetStrategyGameState()->OnResourcesChanged.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);
		GetStrategyGameState()->OnPopulationChanged.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);
		GetStrategyGameState()->OnAssignedWorkersChanged.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);
		GetStrategyGameState()->OnSkyscraperModuleAdded.AddUniqueDynamic(this, &ThisClass::OnSkyscraperModuleAdded);
	}
}

void UBaseStrategyWidget::OnControllerPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (GetRTSCamera())
	{
		GetRTSCamera()->OnBuildableSelected.AddUniqueDynamic(this, &ThisClass::OnBuildableSelected);
		GetRTSCamera()->OnBuildableDeSelected.AddUniqueDynamic(this, &ThisClass::OnBuildableDeSelected);
	}

	BP_OnControllerPawnChanged(OldPawn, NewPawn); 
}

void UBaseStrategyWidget::OnBuildableSelected(ABuildable* SelectedBuildable)
{
	BP_OnBuildableSelected(SelectedBuildable);
}

void UBaseStrategyWidget::OnBuildableDeSelected()
{
	BP_OnBuildableDeSelected();
}

void UBaseStrategyWidget::OnResourcesChanged()
{
	BP_OnResourcesChanged();
}

void UBaseStrategyWidget::OnPopulationChanged()
{
	BP_OnPopulationChanged();
}

void UBaseStrategyWidget::OnAssignedWorkersChanged()
{
	BP_OnAssignedWorkersChanged();
}

void UBaseStrategyWidget::OnStructureBuilt(AStructure* BuiltStructure)
{
	BP_OnStructureBuilt(BuiltStructure);
}

void UBaseStrategyWidget::OnSkyscraperModuleAdded(ASkyscraper* Skyscraper, ASkyscraperModule* AddedModule)
{
	BP_OnSkyscraperModuleAdded(Skyscraper, AddedModule);
}


AStrategyGameState* UBaseStrategyWidget::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

AStrategyGameModeBase* UBaseStrategyWidget::GetStrategyGameMode()
{
	if (StrategyGameMode == nullptr)
	{
		StrategyGameMode = Cast<AStrategyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	return StrategyGameMode;
}

ARTSPlayerController* UBaseStrategyWidget::GetRTSPlayerController()
{
	if (StrategyGameMode == nullptr)
	{
		RTSPlayerController = Cast<ARTSPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	}

	return RTSPlayerController;
}

ARTSCamera* UBaseStrategyWidget::GetRTSCamera()
{
	if (StrategyGameMode == nullptr)
	{
		RTSCamera = Cast<ARTSCamera>(GetRTSPlayerController()->GetRTSCamera());
	}

	return RTSCamera;
}
