// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseStrategyWidget.h"
#include "StructureHorizontalBoxWidget.generated.h"

enum class EStructureCategory : uint8;
class UStructureButtonWidget;
class UHorizontalBox;

/**
 * A Horizontal box that lists all available structures that can be built.
 */
UCLASS(Abstract)
class STRATEGYGAME_API UStructureHorizontalBoxWidget : public UBaseStrategyWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* HorizontalBox;

	UPROPERTY(EditAnywhere, Category="Structure List")
	TSubclassOf<UStructureButtonWidget> StructureButtonClass;

	UPROPERTY(EditAnywhere, Category="Structure List")
	EStructureCategory StructureCategory;

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable, Category="Structure List")
	void AddStructureButton(TSubclassOf<AStructure> InAssignedStructure);

	UFUNCTION(BlueprintCallable, Category="Structure List")
	void AddAllStructuresFromCategory(EStructureCategory InStructureCategory);
};
