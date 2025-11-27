// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureComponent.h"
#include "HousingComponent.generated.h"


class ACitizen;

UCLASS(ClassGroup=(StructureComponents))
class STRATEGYGAME_API UHousingComponent : public UStructureComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHousingComponent();

protected:

	UPROPERTY(EditAnywhere, Category="Housing", meta=(ClampMin=1))
	int32 HousingCapacity = 1;

	UPROPERTY(VisibleAnywhere, Category="Housing")
	TArray<ACitizen*> Residents;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnStructureBuilt(AStructure* BuiltStructure) override;

public:

	virtual void RequestResidents();

	UFUNCTION(BlueprintCallable, Category="Housing")
	void AssignResident(ACitizen* NewResident);

	UFUNCTION(BlueprintPure, Category="Housing")
	TArray<ACitizen*>& GetResidents();

	UFUNCTION(BlueprintPure, Category="Housing")
	int32 GetHousingCapacity() const;

	UFUNCTION(BlueprintPure, Category="Housing")
	bool IsFullCapacity() const;

	UFUNCTION(BlueprintPure, Category="Housing")
	int32 GetNumAvailableResidents() const;
};
