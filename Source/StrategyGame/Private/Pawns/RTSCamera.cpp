// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/RTSCamera.h"

#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARTSCamera::ARTSCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	OnStructureSelectedDelegate.AddUniqueDynamic(this, &ThisClass::OnStructureSelected);
}

// Called when the game starts or when spawned
void ARTSCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARTSCamera::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	GetPlayerController()->GetRTSCamera();
	GetPlayerController()->SetControllerMode(EControllerMode::RTS);
}

void ARTSCamera::OnStructureSelected(ABuildableStructure* Selected)
{
	SelectedStructure = Selected;
}

void ARTSCamera::Move(FVector2D MoveInput)
{
	FVector MovementVector = MoveInput.Y * FVector::ForwardVector + MoveInput.X * FVector::RightVector;
	MovementVector *= MaxMoveSpeed * GetWorld()->DeltaTimeSeconds;

	MovementVector = MovementVector.GetClampedToSize(-MaxMoveSpeed, MaxMoveSpeed);
	
	SetActorLocation(GetActorLocation() + MovementVector);
}

void ARTSCamera::MouseInput()
{
	
}

void ARTSCamera::Zoom(float Input)
{
	FVector HeightVector = FVector::UpVector * Input * MaxZoomSpeed * GetWorld()->DeltaTimeSeconds;

	SetActorLocation(GetActorLocation() + HeightVector);

	if (GetActorLocation().Z > MaxZoomHeight)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, MaxZoomHeight));
	}
	else if (GetActorLocation().Z < MinZoomHeight)
	{
		SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, MinZoomHeight));
	}
}

void ARTSCamera::SelectTarget()
{
	FHitResult Hit = LineTraceToMousePos();

	if (StructureToBuild && StructureToBuild->IsBeingCreated())
	{
		BuildStructure();
		return;
	}
	
	if (Hit.GetActor() && Hit.GetActor()->Implements<USelectionInterface>())
	{
		Execute_Select(Hit.GetActor(), this);
		return;
	}
}

void ARTSCamera::CancelAction()
{
	if (StructureToBuild)
	{
		StructureToBuild->Destroy();
		StructureToBuild = nullptr;
	}
}

void ARTSCamera::BuildStructure()
{
	if (!StructureToBuild) return;
	if (StructureToBuild->IsOverlappingBuildExclusionZone())
	{
		GEngine->AddOnScreenDebugMessage(800, 3.0f, FColor::Red, GetSelectedStructure()->GetDisplayName() + " is overlapping Build Exclusion Zone.");
		return;
	}
	if (!StructureToBuild->HaveEnoughResourcesToBuild())
	{
		GEngine->AddOnScreenDebugMessage(801, 3.0f, FColor::Red, "Not enough materials to build " + GetSelectedStructure()->GetDisplayName());
		return;
	}
	if (!StructureToBuild->IsOverlappingResourceNode() && StructureToBuild->GetNeedsToBeNearResourceNode())
	{
		GEngine->AddOnScreenDebugMessage(800, 3.0f, FColor::Red, GetSelectedStructure()->GetDisplayName() + " needs to be near the correct resource.");
		return;
	}

	ABuildableStructure* NewStructure = GetWorld()->SpawnActor<ABuildableStructure>(StructureToBuild->GetClass());
	NewStructure->SetActorLocation(StructureToBuild->GetActorLocation());
	NewStructure->BeginConstruction();
}

void ARTSCamera::ExitRTSMode()
{
	verify(GetPlayerController()->GetPlayerCharacter())

	CancelAction();

	GetPlayerController()->Possess(GetPlayerController()->GetPlayerCharacter());
}

FHitResult ARTSCamera::LineTraceToMousePos()
{
	FVector TraceStart;
	FVector TraceDirection;
	GetPlayerController()->DeprojectMousePositionToWorld(TraceStart, TraceDirection);
	FVector TraceEnd = TraceStart + TraceDirection * 200000.0f;
	bool TraceComplex = false;
	TArray<AActor*> ActorsToIgnore;
	GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	if (StructureToBuild) ActorsToIgnore.Add(StructureToBuild);
	FHitResult Hit;
	bool IgnoreSelf = true;

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic),
		TraceComplex, ActorsToIgnore, EDrawDebugTrace::None, Hit, IgnoreSelf, FLinearColor::Red, FLinearColor::Green, 0.5f);
	
	return Hit;
}

void ARTSCamera::SelectStructureBlueprint(TSubclassOf<ABuildableStructure> NewBlueprint)
{
	CancelAction();
	StructureToBuild = GetWorld()->SpawnActor<ABuildableStructure>(NewBlueprint);
}

void ARTSCamera::MoveStructureToMousePos()
{
	if (!StructureToBuild) return;

	FHitResult Hit = LineTraceToMousePos();

	FVector NewLocation = Hit.ImpactPoint;
	NewLocation = FVector(FMath::RoundToInt32(NewLocation.X / SnappingSize), FMath::RoundToInt32(NewLocation.Y / SnappingSize), NewLocation.Z);
	NewLocation = FVector(NewLocation.X * SnappingSize, NewLocation.Y * SnappingSize, NewLocation.Z);
	StructureToBuild->SetActorLocation(NewLocation);
}

// Called every frame
void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StructureToBuild && StructureToBuild->IsBeingCreated())
	{
		MoveStructureToMousePos();
	}

	FString MetalString = "Metal: ";
	MetalString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Metal));
	MetalString.Append(" / ");
	MetalString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::Metal));
	GEngine->AddOnScreenDebugMessage(930, 1.0f, FColor::White, MetalString);
	
	FString AlienMaterialString = "Alien Material: ";
	AlienMaterialString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::AlienMaterial));
	AlienMaterialString.Append(" / ");
	AlienMaterialString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::AlienMaterial));
	GEngine->AddOnScreenDebugMessage(931, 1.0f, FColor::White, AlienMaterialString);
	
	FString FoodString = "Food: ";
	FoodString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Food));
	FoodString.Append(" / ");
	FoodString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::Food));
	GEngine->AddOnScreenDebugMessage(932, 1.0f, FColor::White, FoodString);

	FString PowerString = "Power: ";
	PowerString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Power));
	PowerString.Append(" / ");
	PowerString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::Power));
	GEngine->AddOnScreenDebugMessage(933, 1.0f, FColor::White, PowerString);

	FString WorkersString = "Workers: ";
	WorkersString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Workers));
	WorkersString.Append(" / ");
	WorkersString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::Workers));
	GEngine->AddOnScreenDebugMessage(934, 1.0f, FColor::White, WorkersString);

	FString ScientistsString = "Scientists: ";
	ScientistsString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Scientists));
	ScientistsString.Append(" / ");
	ScientistsString.AppendInt(GetStrategyGameState()->GetMaximumResource(EResourceType::Scientists));
	GEngine->AddOnScreenDebugMessage(935, 1.0f, FColor::White, ScientistsString);
}

ARTSPlayerController* ARTSCamera::GetPlayerController()
{
	if (RTSPlayerController == nullptr)
	{
		RTSPlayerController = Cast<ARTSPlayerController>(GetController());
	}

	return RTSPlayerController;
}

AStrategyGameState* ARTSCamera::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

