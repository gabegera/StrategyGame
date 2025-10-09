// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TerminalScreenWidget.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS(Abstract)
class STRATEGYGAME_API UTerminalScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	FText StartingCommandLogText;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnCommandLineTextCommited(const FText& InText, ETextCommit::Type CommitType); 

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	class UEditableText* CommandLine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock* CommandLog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidgetOptional))
	class UWidgetSwitcher* MenuSwitcher;
	
	UFUNCTION(BlueprintCallable)
	void ClearCommandLine(bool bSetFocusAfterClear = true);

	UFUNCTION(BlueprintCallable)
	void ClearCommandLog(bool bSetFocusAfterClear = true);

	UFUNCTION(BlueprintCallable)
	void AddCommandLog(FString InString);
};
