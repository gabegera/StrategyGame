#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, DisplayName="Citizen Type")
enum class ECitizenType : uint8
{
	Worker			UMETA(DisplayName="Worker"),
	Scientist		UMETA(DisplayName="Scientist"),
};

UENUM(BlueprintType, DisplayName="Citizen State")
enum class ECitizenState : uint8
{
	Roaming			UMETA(DisplayName="Roaming"),
	GoingHome		UMETA(DisplayName="Going Home"),
	AtHome			UMETA(DisplayName="At Home"),
	GoingToWork		UMETA(DisplayName="Going to Work"),
	Working			UMETA(DisplayName="Working"),
};