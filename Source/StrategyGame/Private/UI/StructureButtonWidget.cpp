// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StructureButtonWidget.h"

#include "Building/Structure.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Game/StrategyGameInstance.h"
#include "Game/UnlocksSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

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
	if (IsRequiredUpgradeUnlocked())
	{
		FTransform SpawnTransform = FTransform(FVector::ZeroVector);
		AStructure* SpawnedStructure = GetWorld()->SpawnActorDeferred<AStructure>(AssignedStructure, SpawnTransform);
		SpawnedStructure->SetStructureState(EStructureState::BeingPlaced);
		SpawnedStructure->FinishSpawning(SpawnTransform);

		GetGameInstance<UStrategyGameInstance>()->OnStructureSelected.Broadcast(SpawnedStructure);
	}
	else
	{
		UKismetSystemLibrary::PrintString(GetWorld(), "The Upgrade: " + RequiredUpgrade->GetTitle() + " is required.");
	}

}

TSubclassOf<AStructure> UStructureButtonWidget::GetAssignedStructure()
{
	return AssignedStructure;
}

UUpgradeDataAsset* UStructureButtonWidget::GetRequiredUpgrade() const
{
	return RequiredUpgrade;
}

bool UStructureButtonWidget::IsRequiredUpgradeUnlocked() const
{
	if (RequiredUpgrade)
	{
		return GetGameInstance()->GetSubsystem<UUnlocksSubsystem>()->IsUpgradeUnlocked(RequiredUpgrade);
	}

	return true;
}

void UStructureButtonWidget::SetAssignedStructure(TSubclassOf<AStructure> InStructureClass)
{
	AssignedStructure = InStructureClass;
}
