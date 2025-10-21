#pragma once

#include "CoreMinimal.h"
#include "RaidSettings.generated.h"

class UResourceDataAsset;

USTRUCT(BlueprintType)
struct FRaidSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Settings")
	TMap<UResourceDataAsset*, int32> RaidRewards;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Raid Settings")
	float HoursToTravelToRaid;
};