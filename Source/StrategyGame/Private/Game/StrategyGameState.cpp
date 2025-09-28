// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StrategyGameState.h"

#include "Building/Structure.h"
#include "Kismet/GameplayStatics.h"


AStrategyGameState::AStrategyGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	StructureBuiltDelegate.AddUniqueDynamic(this, &ThisClass::OnStructureBuilt);
	StructureDestroyedDelegate.AddUniqueDynamic(this, &ThisClass::OnStructureDestroyed);
}

void AStrategyGameState::BeginPlay()
{
	Super::BeginPlay();

	// BuiltStructures = FindAllStructures();
}

void AStrategyGameState::OnStructureBuilt(AStructure* BuiltStructure)
{
	// BuiltStructures.AddUnique(BuiltStructure);
}

void AStrategyGameState::OnStructureDestroyed(AStructure* BuiltStructure)
{
	// BuiltStructures.Remove(BuiltStructure);
	// BuiltStructures.Shrink();
}



TArray<AStructure*> AStrategyGameState::FindAllStructures()
{
	TArray<AActor*> FoundStructures;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStructure::StaticClass(), FoundStructures);

	TArray<AStructure*> OutArray;
	for (AActor* FoundStructure : FoundStructures)
	{
		OutArray.AddUnique(Cast<AStructure>(FoundStructure));
	}

	return OutArray;
}

void AStrategyGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

AStrategyGameModeBase* AStrategyGameState::GetStrategyGameMode()
{
	if (StrategyGameMode == nullptr)
	{
		StrategyGameMode = Cast<AStrategyGameModeBase>(GetWorld()->GetAuthGameMode());
	}

	return StrategyGameMode;
}

ETimeScale AStrategyGameState::SetTimeScale(ETimeScale NewTimeScale)
{
	switch (NewTimeScale)
	{
	case ETimeScale::OneTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
		break;
	case ETimeScale::TwoTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 2.0f);
		break;
	case ETimeScale::ThreeTimesSpeed:
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 3.0f);
		break;
	}
	
	OnTimeScaleChanged.Broadcast(NewTimeScale);
	
	return TimeScale = NewTimeScale;
}

