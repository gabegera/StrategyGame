// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructureComponent.h"
#include "WorkersComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STRATEGYGAME_API UWorkersComponent : public UStructureComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWorkersComponent();

protected:

	// If set to 0 no workers are needed.
	UPROPERTY(EditAnywhere, Category="Workers")
	int32 MaxNumOfWorkers = 0;

	// If set to true that citizen type is allowed to work here.
	UPROPERTY(EditAnywhere, Category="Workers", meta=(EditCondition="MaxNumOfWorkers > 0", EditConditionHides))
	TMap<ECitizenType, bool> AllowedWorkerTypes;

	UPROPERTY(VisibleAnywhere, Category="Workers", meta=(EditCondition="MaxNumOfWorkers > 0", EditConditionHides))
	TSet<ACitizen*> AssignedWorkers;

	// Called when the game starts
	virtual void BeginPlay() override;

	void FetchAllCitizenTypes();

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void OnStructureBuilt(AStructure* BuiltStructure) override;

public:

	virtual void RequestWorker(ECitizenType RequestedWorkerType);

	virtual void RequestNumOfWorkers(int32 RequestedNumOfWorkers, ECitizenType RequestedWorkerType);

	UFUNCTION(BlueprintCallable, Category="Workers")
	void AssignWorker(ACitizen* InWorker);

	UFUNCTION(BlueprintPure)
	bool IsFullCapacity() const;

	UFUNCTION(BlueprintPure)
	bool IsWorkerAllowed(ECitizenType InCitizenType) const;

	UFUNCTION(BlueprintPure)
	int32 GetMaxNumOfWorkers() const;

	UFUNCTION(BlueprintPure)
	int32 GetNumOfWorkingWorkers() const;

	UFUNCTION(BlueprintPure)
	TSet<ACitizen*> GetAssignedWorkers() const;

	UFUNCTION(BlueprintPure)
	int32 GetNumOfAssignedWorkers(ECitizenType InCitizenType) const;

	// Returns between 0 and 1 what percentage of workers are currently working.
	UFUNCTION(BlueprintPure)
	float GetWorkersPercentage() const;
};
