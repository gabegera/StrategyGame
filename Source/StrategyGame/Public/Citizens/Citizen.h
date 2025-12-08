// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CitizenInterface.h"
#include "Citizen.generated.h"

enum class ECitizenType : uint8;
enum class ECitizenState : uint8;
class UWorkersComponent;
class UHousingComponent;
class AStructure;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWorkplaceAssignedSignature, AStructure*, AssignedWorkplace);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWorkplaceClearedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHomeAssignedSignature, AStructure*, AssignedHome);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHomeClearedSignature);

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
	ECitizenState CitizenState;

	UPROPERTY(VisibleAnywhere, Category="Citizen")
	AStructure* Workplace;

	UPROPERTY(VisibleAnywhere, Category="Citizen")
	AStructure* Home;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnStructureBuilt(AStructure* NewStructure);

	UFUNCTION()
	virtual void OnStructureDestroyed(AStructure* DestroyedStructure);

	UFUNCTION()
	virtual void OnResidentRequested(AStructure* StructureSource, UHousingComponent* Housing);

	UFUNCTION()
	virtual void OnWorkerRequested(AStructure* StructureSource, UWorkersComponent* WorkersComponent, ECitizenType RequestedCitizenType);

public:

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWorkplaceAssignedSignature OnWorkplaceAssigned;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FWorkplaceClearedSignature OnWorkplaceCleared;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FHomeAssignedSignature OnHomeAssigned;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FHomeClearedSignature OnHomeCleared;

	virtual ECitizenType TryGetCitizenType_Implementation() override;

	virtual ECitizenState TrySetCitizenState_Implementation(ECitizenState InCitizenState) override;

	virtual ECitizenState TryGetCitizenState_Implementation() override;

	virtual AStructure* TryGetHome_Implementation() override;

	virtual AStructure* TryGetWorkplace_Implementation() override;

	virtual bool TryGetIsEmployed_Implementation() override;

	virtual bool TryGetIsHomeless_Implementation() override;

	virtual void TryToEnterStructure_Implementation(AStructure* TargetStructure) override;

	virtual void TryToExitStructure_Implementation() override;

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void ClearHome();

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void ClearWorkplace();

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void EnterStructure(AStructure* TargetStructure);

	UFUNCTION(BlueprintCallable, Category="Citizen")
	void ExitStructure();

	UFUNCTION(BlueprintCallable, Category="Citizen")
	ECitizenState SetCitizenState(ECitizenState InCitizenState);

	UFUNCTION(BlueprintPure, Category="Citizen")
	ECitizenState GetCitizenState() const;

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