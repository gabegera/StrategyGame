// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StructureHorizontalBoxWidget.h"

#include "StrategyStatics.h"
#include "Building/Structure.h"
#include "Components/HorizontalBox.h"
#include "Game/UnlocksSubsystem.h"
#include "UI/StructureButtonWidget.h"

void UStructureHorizontalBoxWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	FetchAllStructuresFromCategory(StructureCategory);
}

void UStructureHorizontalBoxWidget::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FetchAllStructuresFromCategory(StructureCategory);
}

void UStructureHorizontalBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// for (const TPair Structure : BuildableStructures)
	// {
	// 	if (Structure.Key && Structure.Value == true)
	// 	{
	// 		AddStructureButton(Structure.Key);
	// 	}
	// }
}

void UStructureHorizontalBoxWidget::AddStructureButton(const TSubclassOf<AStructure> InAssignedStructure)
{
	if (!StructureButtonClass || !HorizontalBox) return;

	FString NewButtonName = InAssignedStructure.GetDefaultObject()->GetStructureName().ToString() + "Button";
	NewButtonName.RemoveSpacesInline();
	if (UStructureButtonWidget* NewStructureButton = CreateWidget<UStructureButtonWidget>(this, StructureButtonClass, FName(*NewButtonName)))
	{
		NewStructureButton->SetAssignedStructure(InAssignedStructure);
		HorizontalBox->AddChildToHorizontalBox(NewStructureButton);
	}
}

void UStructureHorizontalBoxWidget::FetchAllStructuresFromCategory(const EStructureCategory InStructureCategory)
{
	for (TSubclassOf<AStructure> StructureClass : UStrategyStatics::GetAllStructureClasses())
	{
		if (StructureClass.GetDefaultObject()->GetStructureCategory() == InStructureCategory)
		{
			BuildableStructures.FindOrAdd(StructureClass, true);
		}
	}
}
