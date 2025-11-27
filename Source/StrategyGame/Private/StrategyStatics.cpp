// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyStatics.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Building/Structure.h"
#include "DataAssets/ResourceDataAsset.h"
#include "Game/StrategyGameInstance.h"
#include "Turrets/RemoteControlTurret.h"

class IAssetRegistry;
class FAssetRegistryModule;

int32 UStrategyStatics::GetGridSize(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (const UStrategyGameInstance* GameInstance = World->GetGameInstance<UStrategyGameInstance>())
	{
		return GameInstance->GetGridSize();
	}
	
	return 1;
}

TArray<UResourceDataAsset*> UStrategyStatics::GetAllResources()
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssetsByClass(UResourceDataAsset::StaticClass()->GetClassPathName(), AssetDataList);

	TArray<UResourceDataAsset*> OutArray;
	for (const FAssetData& AssetData : AssetDataList)
	{
		OutArray.AddUnique(Cast<UResourceDataAsset>(AssetData.GetAsset()));
	}

	return OutArray;
}

UResourceDataAsset* UStrategyStatics::GetResourceByName(const FString& InName)
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssetsByClass(UResourceDataAsset::StaticClass()->GetClassPathName(), AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		UResourceDataAsset* Resource = Cast<UResourceDataAsset>(AssetData.GetAsset());
		if (Resource->GetResourceName().ToString().ToLower() == InName.ToLower())
		{
			return Resource;
		}
	}

	return nullptr;
}

TSet<TSubclassOf<AStructure>> UStrategyStatics::GetAllStructureClasses()
{
	TArray<UClass*> DerivedClasses;
	GetDerivedClasses(AStructure::StaticClass(), DerivedClasses);

	TSet<TSubclassOf<AStructure>> OutSet;
	for (UClass* Class : DerivedClasses)
	{
		if (!Class->IsNative() && !Class->IsChildOf(ARemoteControlTurret::StaticClass()))
		{
			OutSet.Add(Class);
		}
	}

	return OutSet;

	// const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	// const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	//
	// TArray<FAssetData> AssetDataList;
	// AssetRegistry.GetAssetsByClass(AStructure::StaticClass()->GetClassPathName(), AssetDataList);
	//
	// TSet<TSubclassOf<AStructure>> OutSet;
	// for (const FAssetData& AssetData : AssetDataList)
	// {
	// 	OutSet.Add(AssetData.GetClass());
	// }
	//
	// return OutSet;
}