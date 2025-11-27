// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StructureComponent.h"

#include "Building/Structure.h"
#include "Game/StrategyGameInstance.h"
#include "Game/TimeSubsystem.h"


// Sets default values for this component's properties
UStructureComponent::UStructureComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = false;

	OwningStructure = Cast<AStructure>(GetOwner());
}


// Called when the game starts
void UStructureComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningStructure = Cast<AStructure>(GetOwner());

	UStrategyGameInstance* GameInstance = GetWorld()->GetGameInstance<UStrategyGameInstance>();
	GameInstance->OnStructureBuilt.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
	GameInstance->OnStructureDestroyed.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);
	GameInstance->GetSubsystem<UTimeSubsystem>()->OnTimePassed.AddUniqueDynamic(this, &ThisClass::OnTimePassed);
}

void UStructureComponent::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

void UStructureComponent::OnStructureBuilt(AStructure* BuiltStructure)
{
	if (BuiltStructure == OwningStructure)
	{
		Activate();
	}
}

void UStructureComponent::OnStructureDestroyed(AStructure* DestroyedStructure)
{
	if (DestroyedStructure == OwningStructure)
	{
		Deactivate();
	}
}

void UStructureComponent::OnTimePassed(float HoursPassed)
{

}

