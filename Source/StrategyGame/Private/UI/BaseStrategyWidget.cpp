// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/BaseStrategyWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Player/StrategyPlayerController.h"

void UBaseStrategyWidget::NativeConstruct()
{
	StrategyPlayerController = Cast<AStrategyPlayerController>(GetWorld()->GetFirstPlayerController());
}

AStrategyPlayerController* UBaseStrategyWidget::GetStrategyPlayerController() const
{
	return StrategyPlayerController;
}
