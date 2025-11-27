// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseStrategyWidget.h"
#include "StructureButtonWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * UI button used to build structures in RTS mode.
 */
UCLASS(Abstract)
class STRATEGYGAME_API UStructureButtonWidget : public UBaseStrategyWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton* Button;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* ButtonText;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetAssignedStructure, Category="Structure Button", meta=(ExposeOnSpawn))
	TSubclassOf<AStructure> AssignedStructure;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetRequiredUpgrade, Category="Structure Button", meta=(ExposeOnSpawn))
	UUpgradeDataAsset* RequiredUpgrade;

	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnButtonClicked();

public:

	UFUNCTION(BlueprintPure, Category="Structure Button")
	TSubclassOf<AStructure> GetAssignedStructure();

	UFUNCTION(BlueprintPure, Category="Structure Button")
	UUpgradeDataAsset* GetRequiredUpgrade() const;

	UFUNCTION(BlueprintPure, Category="Structure Button")
	bool IsRequiredUpgradeUnlocked() const;

	UFUNCTION(BlueprintCallable, Category="Structure Button")
	void SetAssignedStructure(TSubclassOf<AStructure> InStructureClass);
	
};
