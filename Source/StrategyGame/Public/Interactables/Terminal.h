// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "Components/WidgetComponent.h"
#include "Terminal.generated.h"

class UCameraComponent;

UENUM(BlueprintType)
enum class ECommandType : uint8
{
	OpenNewMenu				UMETA(DisplayName="Open New Menu"),
	RemotelyTriggerObject	UMETA(DisplayName="Remotely Trigger Object"),
};

USTRUCT(BlueprintType)
struct FTerminalCommand
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command")
	ECommandType CommandType;

	// The string that needs entered for the command to trigger. Case Insensitive.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command")
	FString Command;

	// The menu to open when the command is entered.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command", meta=(EditCondition="CommandType == ECommandType::OpenNewMenu", EditConditionHides))
	int32 NewMenuIndex = 1;

	// The Display name for the new menu. Only used for printing strings.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command", meta=(EditCondition="CommandType == ECommandType::OpenNewMenu", EditConditionHides))
	FString NewMenuName = "[EMPTY MENU NAME]";

	// The Object to trigger when the command is entered.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command", meta=(EditCondition="CommandType == ECommandType::RemotelyTriggerObject", EditConditionHides))
	TSet<AActor*> ObjectsToRemotelyTrigger;

	// The Display name for the object to be remotely triggered. Only used for printing strings.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Terminal Command", meta=(EditCondition="CommandType == ECommandType::RemotelyTriggerObject", EditConditionHides))
	FString RemoteObjectName = "[EMPTY OBJECT NAME]";
	
	FString GetCommandHelpString() const
	{
		FString OutString = Command;
		OutString.Append(": ");

		switch (CommandType)
		{
		case ECommandType::OpenNewMenu:
			
			OutString.Append("Opens the ");
			OutString.Append(NewMenuName);
			OutString.Append(" menu.");
			break;
			
		case ECommandType::RemotelyTriggerObject:
			OutString.Append("Remotely triggers the ");
			OutString.Append(RemoteObjectName);
			OutString.Append(" object.");
			break;
			
		default:
			OutString.Append("Unknown Command");
			break;
		}
		
		return OutString;
	}

	bool operator==(const FTerminalCommand& Other) const
	{
		return Command == Other.Command;
	}
};

uint32 GetTypeHash(const FTerminalCommand& Command);

UCLASS()
class STRATEGYGAME_API ATerminal : public AInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATerminal();

protected:

	// The component that renders the terminal UI.
	UPROPERTY(EditAnywhere, BlueprintGetter=GetScreenWidget, Category="Terminal|Components")
	UWidgetComponent* ScreenWidget;

	// The camera that will be looking at the terminal when interacted with.
	UPROPERTY(EditAnywhere, Category="Terminal|Components")
	UCameraComponent* TerminalCamera;

	UPROPERTY(BlueprintGetter=GetTerminalScreenWidget, Category="Terminal")
	class UTerminalScreenWidget* TerminalScreenWidget;
 
	UPROPERTY(EditAnywhere, Category="Terminal")
	TSet<FTerminalCommand> TerminalCommands;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnInteract(APlayerCharacter* InteractInstigator) override;

	UFUNCTION()
	void OnCommandLineTextCommited(const FText& InText, ETextCommit::Type CommitType); 

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UWidgetComponent* GetScreenWidget() const { return ScreenWidget; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UTerminalScreenWidget* GetTerminalScreenWidget() const { return TerminalScreenWidget; }
};
