// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "CitizenAIController.generated.h"

class AStructure;

UCLASS(Abstract)
class STRATEGYGAME_API ACitizenAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACitizenAIController();

protected:
	
	// ------ BLACKBOARD KEYS ------

	const FName BBKey_WorkplacePosition = TEXT("WorkPosition");
	const FName BBKey_HousePosition = TEXT("HousePosition");

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void OnPawnAssignedHome(AStructure* AssignedHome);
	
	UFUNCTION()
	virtual void OnPawnAssignedWorkplace(AStructure* AssignedWorkplace);
};
