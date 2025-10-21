// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/BaseStrategyWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Player/RTSPlayerController.h"

void UBaseStrategyWidget::NativeConstruct()
{
	RTSPlayerController = Cast<ARTSPlayerController>(UGameplayStatics::GetActorOfClass(this, ARTSPlayerController::StaticClass()));
}