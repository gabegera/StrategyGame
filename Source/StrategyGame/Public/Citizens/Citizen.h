// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CitizenInterface.h"
#include "Citizen.generated.h"

class AStructure;

UENUM(BlueprintType, DisplayName="Citizen Type")
enum class ECitizenType : uint8
{
	Worker			UMETA(DisplayName="Worker"),
	Scientist		UMETA(DisplayName="Scientist"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorkplaceAssignedSignature, AStructure*, AssignedWorkplace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHomeAssignedSignature, AStructure*, AssignedHome);

UCLASS(Abstract)
class STRATEGYGAME_API ACitizen : public ACharacter, public ICitizenInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACitizen();

protected:

	UPROPERTY(VisibleAnywhere, Category="Citizen")
	ECitizenType CitizenType;

	UPROPERTY(VisibleAnywhere, Category="Citizen")
	AStructure* Workplace;

	UPROPERTY(VisibleAnywhere, Category="Citizen")
	AStructure* Home;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWorkplaceAssignedSignature OnWorkplaceAssigned;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FHomeAssignedSignature OnHomeAssigned;
	
	virtual bool TryGetIsEmployed_Implementation() override;

	virtual bool TryGetIsHomeless_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void SetCitizenType(ECitizenType NewCitizenType);

	UFUNCTION(BlueprintPure, Category="Citizen")
	ECitizenType GetCitizenType() const;

	UFUNCTION(BlueprintPure, Category="Citizen")
	AStructure* GetWorkplace() const;

	UFUNCTION(BlueprintPure, Category="Citizen")
	AStructure* GetHome() const;

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void AssignWorkplace(AStructure* InWorkplace);

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void AssignHome(AStructure* InHome);

	UFUNCTION(BlueprintPure, Category="Citizen")
	bool IsEmployed() const;	

	UFUNCTION(BlueprintPure, Category="Citizen")
	bool IsHomeless() const;


};