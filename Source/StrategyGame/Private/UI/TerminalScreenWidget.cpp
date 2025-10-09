// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TerminalScreenWidget.h"

#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UTerminalScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CommandLine->OnTextCommitted.AddUniqueDynamic(this, &ThisClass::OnCommandLineTextCommited);

	StartingCommandLogText = CommandLog->GetText();
}

FReply UTerminalScreenWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CommandLine->SetFocus();
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UTerminalScreenWidget::OnCommandLineTextCommited(const FText& InText, ETextCommit::Type CommitType)
{
	if (CommitType != ETextCommit::Type::OnEnter) return;

	if (InText.ToString().ToLower() == "exit")
	{
		ClearCommandLine(false);
	}
	else
	{
		ClearCommandLine(true);
	}
	
}

void UTerminalScreenWidget::ClearCommandLine(bool bSetFocusAfterClear)
{
	CommandLine->SetText(FText());

	if (bSetFocusAfterClear)
	{
		CommandLine->SetFocus();		
	}

}

void UTerminalScreenWidget::ClearCommandLog(bool bSetFocusAfterClear)
{
	CommandLog->SetText(StartingCommandLogText);

	if (bSetFocusAfterClear)
	{
		CommandLine->SetFocus();		
	}
}

void UTerminalScreenWidget::AddCommandLog(FString InString)
{
	FString LogText = CommandLog->GetText().ToString();
	LogText.Append("\n");
	LogText.Append(InString);

	CommandLog->SetText(FText::FromString(LogText));
}
