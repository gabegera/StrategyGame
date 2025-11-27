// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "CitizenAIController.generated.h"

class ACitizen;
class AStructure;

UCLASS(Abstract)
class STRATEGYGAME_API ACitizenAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACitizenAIController();

protected:

	UPROPERTY()
	ACitizen* CitizenPawn;

	UPROPERTY(EditAnywhere, Category="Citizen AI")
	float RoamingRadius = 1000.0f;
	
	// ------ BLACKBOARD KEYS ------

	const FName BBKey_TargetDestination = TEXT("TargetDestination");
	const FName BBKey_WorkplaceLocation = TEXT("WorkplaceLocation");
	const FName BBKey_HomeLocation = TEXT("HomeLocation");

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* BehaviorTree;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	virtual void OnTimePassed(float HoursPassed);

	UFUNCTION()
	virtual void OnWorkTimeStarted();

	UFUNCTION()
	virtual void OnWorkTimeEnded();

	UFUNCTION()
	virtual void OnPawnAssignedHome(AStructure* AssignedHome);

	UFUNCTION()
	virtual void OnPawnClearedHome();
	
	UFUNCTION()
	virtual void OnPawnAssignedWorkplace(AStructure* AssignedWorkplace);

	UFUNCTION()
	virtual void OnPawnClearedWorkplace();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	UFUNCTION(BlueprintCallable, Category="Citizen AI")
	void GoToHome();

	UFUNCTION(BlueprintCallable, Category="Citizen AI")
	void GoToWork();

	UFUNCTION(BlueprintCallable, Category="Citizen AI")
	void MoveToRandomPointInRadius(FVector Origin, float Radius);

public:

	UFUNCTION(BlueprintPure, Category="Citizen AI")
	ACitizen* GetCitizenPawn() const;

	UFUNCTION(BlueprintPure, Category="Citizen AI")
	float GetDistanceToStructureEntrance(const AStructure* InStructure) const;

	UFUNCTION(BlueprintPure, Category="Citizen AI")
	bool IsTimeToWork() const;
};
