// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CitizenInterface.generated.h"

enum class ECitizenType : uint8;
enum class ECitizenState : uint8;
class AStructure;

// This class does not need to be modified.
UINTERFACE()
class UCitizenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STRATEGYGAME_API ICitizenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	ECitizenType TryGetCitizenType();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	ECitizenState TrySetCitizenState(ECitizenState InCitizenState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	ECitizenState TryGetCitizenState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	AStructure* TryGetHome();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	AStructure* TryGetWorkplace();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	bool TryGetIsEmployed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	bool TryGetIsHomeless();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	void TryToEnterStructure(AStructure* TargetStructure);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Citizen Interface")
	void TryToExitStructure();
};
