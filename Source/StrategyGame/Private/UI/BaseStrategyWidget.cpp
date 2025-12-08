// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/BaseStrategyWidget.h"

#include "Game/StrategyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/StrategyPlayerController.h"

void UBaseStrategyWidget::NativeConstruct()
{
	StrategyPlayerController = Cast<AStrategyPlayerController>(GetWorld()->GetFirstPlayerController());

	UStrategyGameInstance* GameInstance = GetGameInstance<UStrategyGameInstance>();
	GameInstance->OnStructureSelected.AddUniqueDynamic(this, &ThisClass::OnStructureSelected);
	GameInstance->OnStructureDeSelected.AddUniqueDynamic(this, &ThisClass::OnStructureDeSelected);
}

AStrategyPlayerController* UBaseStrategyWidget::GetStrategyPlayerController() const
{
	return StrategyPlayerController;
}
