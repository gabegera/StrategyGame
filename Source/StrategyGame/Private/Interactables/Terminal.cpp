// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Terminal.h"

#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Player/PlayerCharacter.h"
#include "UI/TerminalScreenWidget.h"


uint32 GetTypeHash(const FTerminalCommand& Command)
{
	return GetTypeHash(Command.Command);
}

// Sets default values
ATerminal::ATerminal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ScreenWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen Widget"));
	ScreenWidget->SetupAttachment(StaticMesh);

	TerminalCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TerminalCamera->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void ATerminal::BeginPlay()
{
	Super::BeginPlay();

	TerminalScreenWidget = Cast<UTerminalScreenWidget>(ScreenWidget->GetWidget());
	if (TerminalScreenWidget)
	{
		if (TerminalScreenWidget->CommandLine)
		{
			TerminalScreenWidget->CommandLine->OnTextCommitted.AddUniqueDynamic(this, &ThisClass::OnCommandLineTextCommited);
		}
	}
}

void ATerminal::OnInteract(APlayerCharacter* InteractInstigator)
{
	Super::OnInteract(InteractInstigator);

	if (InteractInstigator && InteractInstigator->GetPlayerController())
	{
		ARTSPlayerController* PlayerController = InteractInstigator->GetPlayerController();
		
		PlayerController->SetControllerMode(EControllerMode::Terminal);
		PlayerController->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 2);
		PlayerController->SetShowMouseCursor(true);

		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(TerminalScreenWidget->CommandLine->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		InputMode.SetHideCursorDuringCapture(false);
		PlayerController->SetInputMode(InputMode);
	}
}

void ATerminal::OnCommandLineTextCommited(const FText& InText, ETextCommit::Type CommitType)
{
	if (CommitType != ETextCommit::Type::OnEnter) return;

	FString InString = InText.ToString().ToLower();
	
	for (FTerminalCommand TerminalCommand : TerminalCommands)
	{
		if (TerminalCommand.Command.ToLower() != InString) continue;

		switch (TerminalCommand.CommandType)
		{
		case ECommandType::OpenNewMenu:

			if (TerminalScreenWidget->MenuSwitcher)
			{
				TerminalScreenWidget->MenuSwitcher->SetActiveWidgetIndex(TerminalCommand.NewMenuIndex);
			}
			return;
			
		case ECommandType::RemotelyTriggerObject:
			
			return;
			
		default:
			return;
		}
	}

	if (InString == "help")
	{
		for (FTerminalCommand TerminalCommand : TerminalCommands)
		{
			TerminalScreenWidget->AddCommandLog(TerminalCommand.GetCommandHelpString());
		}
		return;
	}

	if (InString == "clear")
	{
		TerminalScreenWidget->ClearCommandLog();
		return;
	}	

	if (InString == "exit")
	{
		ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController());

		checkf(PlayerController,  TEXT("ARTSPlayerController is null. ATerminal::OnCommandLineTextCommited."))

		PlayerController->Exit();

		return;
	}	

	TerminalScreenWidget->AddCommandLog(InString.Append(" [ERROR: UNKNOWN COMMAND]"));
}

// Called every frame
void ATerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

