// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StructureButtonWidget.h"

#include "Building/Structure.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/StrategyGameInstance.h"

void UStructureButtonWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (AssignedStructure && ButtonText)
	{
		ButtonText->SetText(Cast<AStructure>(AssignedStructure.GetDefaultObject())->GetStructureName());
	}
	else
	{
		ButtonText->SetText(FText::FromString("EMPTY STRUCTURE NAME"));
	}
}

void UStructureButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddUniqueDynamic(this, &ThisClass::OnButtonClicked);
}

void UStructureButtonWidget::OnButtonClicked()
{
	FTransform SpawnTransform = FTransform(FVector::ZeroVector);
	AStructure* SpawnedStructure = GetWorld()->SpawnActorDeferred<AStructure>(AssignedStructure, SpawnTransform);
	SpawnedStructure->SetStructureState(EStructureState::BeingPlaced);
	SpawnedStructure->FinishSpawning(SpawnTransform);
	
	GetGameInstance<UStrategyGameInstance>()->OnStructureSelected.Broadcast(SpawnedStructure);
}

TSubclassOf<AStructure> UStructureButtonWidget::GetAssignedStructure()
{
	return AssignedStructure;
}

void UStructureButtonWidget::SetAssignedStructure(TSubclassOf<AStructure> InStructureClass)
{
	AssignedStructure = InStructureClass;
}
