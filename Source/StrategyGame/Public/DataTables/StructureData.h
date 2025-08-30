#pragma once

#include "CoreMinimal.h"
#include "GameStates/StrategyGameState.h"
#include "Engine/DataTable.h"
#include "StructureData.generated.h"

USTRUCT(Blueprintable)
struct FStructureData : public FTableRowBase
{
	GENERATED_BODY()
	
	// ------ RESOURCES ------

	// How much this structure increases population capacity;
	UPROPERTY(EditAnywhere, Category="Population", meta=(ClampMin=0))
	int32 AdditionalPopulationCapacity = 0;

	UPROPERTY(EditAnywhere, Category="Resources")
	bool bGeneratesResources = false;

	UPROPERTY(EditAnywhere, Category="Resources|Resource Generation", meta=(EditCondition="bGeneratesResources", EditConditionHides))
	TMap<EResourceType, float> ResourcesToGeneratePerSecond;

	UPROPERTY(EditAnywhere, Category="Resources")
	bool bConsumesResources = false;

	UPROPERTY(EditAnywhere, Category="Resources|Resource Consumption", meta=(EditCondition="bConsumesResources", EditConditionHides))
	bool bConsumesResourceFromNearbyNode = false;

	UPROPERTY(EditAnywhere, Category="Resources|Resource Consumption", meta=(EditCondition="bConsumesResources", EditConditionHides))
	TMap<EResourceType, float> ResourcesToConsumePerSecond;

	UPROPERTY(EditAnywhere, Category="Resources")
	bool bIncreasesStorageCapacity = false;

	UPROPERTY(EditAnywhere, Category="Resources|Resource Storage", meta=(EditCondition="bIncreasesStorageCapacity", EditConditionHides))
	TMap<EResourceType, int32> ResourcesToIncreaseStorage;

	// ------ WORKERS ------

	UPROPERTY(EditAnywhere, Category="Workers")
	bool bAllowWorkerEmployment = false;
	
	UPROPERTY(EditAnywhere, Category="Workers")
	bool bAllowScientistEmployment = false;

	UPROPERTY(EditAnywhere, Category="Workers", meta=(EditCondition="bAllowWorkerEmployment || bAllowScientistEmployment", EditConditionHides))
	int32 MaxWorkerCapacity = 15;
};