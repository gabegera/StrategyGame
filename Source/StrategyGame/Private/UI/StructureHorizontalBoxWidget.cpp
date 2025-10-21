// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StructureHorizontalBoxWidget.h"

#include "Building/Structure.h"
#include "Components/HorizontalBox.h"
#include "Game/UpgradesSubsystem.h"
#include "UI/StructureButtonWidget.h"

void UStructureHorizontalBoxWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AddAllStructuresFromCategory(StructureCategory);
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

void UStructureHorizontalBoxWidget::AddAllStructuresFromCategory(EStructureCategory InStructureCategory)
{
	UUpgradesSubsystem* UpgradesSubsystem = GetGameInstance()->GetSubsystem<UUpgradesSubsystem>();

	if (UpgradesSubsystem->GetUnlockedStructuresOfCategory(InStructureCategory).IsEmpty()) return;
	
	for (const TSubclassOf Structure : UpgradesSubsystem->GetUnlockedStructuresOfCategory(InStructureCategory))
	{
		AddStructureButton(Structure);
	}
}
