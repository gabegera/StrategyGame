// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/RTSCamera.h"

#include "Characters/PlayerCharacter.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ARTSCamera::ARTSCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);	
	
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ARTSCamera::BeginPlay()
{
	Super::BeginPlay();
	
	SpringArm->TargetArmLength = (ZoomDistanceMax + ZoomDistanceMin) / 2;
	ZoomDistanceTarget = SpringArm->TargetArmLength;
	UpdateCameraPitch();

	GetStrategyGameState()->ResourcesChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);
	GetStrategyGameState()->ResourcesChangedDelegate.AddUniqueDynamic(this, &ThisClass::OnResourcesChanged);

	
}

void ARTSCamera::OnStructureSelected(ABuildable* Selected)
{
	SelectedBuildable = Selected;
}

void ARTSCamera::OnResourcesChanged()
{
	GEngine->AddOnScreenDebugMessage(200, 3.0f, FColor::Yellow, "Resources Were Updated");
}

void ARTSCamera::Move(FVector2D MoveInput)
{
	float MoveSpeedMultiplier = 1000.0f;
	FVector MovementVector = MoveInput.Y * GetActorForwardVector().RotateAngleAxis(GetActorRotation().Pitch, GetActorRightVector()) + MoveInput.X * GetActorRightVector();
	MovementVector *= MoveSpeed * MoveSpeedMultiplier * GetWorld()->DeltaTimeSeconds;
	// Used to make the camera move faster when it's more zoomed out, and slower when it's more zoomed in.
	MovementVector *= GetZoomDistanceAlpha() + 1.0f;

	// MovementVector = MovementVector.GetClampedToSize(-MoveSpeed, MoveSpeed);
	
	SetActorLocation(GetActorLocation() + MovementVector);
}

void ARTSCamera::RotateCamera(float Input)
{
	float RotationAmount = Input * CameraRotationSpeed * GetWorld()->DeltaTimeSeconds;

	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw + RotationAmount, GetActorRotation().Roll));
}

void ARTSCamera::UpdateCameraPitch()
{
	FRotator MinRotation = GetActorRotation();
	MinRotation.Pitch = -CameraPitchAtMaxZoom;
	FRotator MaxRotation = GetActorRotation();
	MaxRotation.Pitch = -CameraPitchAtMinZoom;

	FRotator NewRotation = FRotator(FMath::Lerp(MaxRotation, MinRotation, GetZoomDistanceAlpha()));
	SetActorRotation(NewRotation);
}

void ARTSCamera::Zoom(float Input)
{
	float ZoomSpeedMultiplier = 1000.0f;
	float ZoomValue = Input * ZoomSpeed * ZoomSpeedMultiplier * GetWorld()->DeltaTimeSeconds;
	
	// Used to make the camera zoom faster when it's more zoomed out, and slower when it's more zoomed in.
	ZoomValue *= GetZoomDistanceAlpha() + 1.0f;

	ZoomDistanceTarget = FMath::Clamp(ZoomDistanceTarget + ZoomValue, ZoomDistanceMin, ZoomDistanceMax);
}

void ARTSCamera::UpdateZoom()
{
	SpringArm->TargetArmLength = FMath::InterpEaseInOut(SpringArm->TargetArmLength, ZoomDistanceTarget, GetWorld()->DeltaTimeSeconds * ZoomEasingSpeed, ZoomEasing);
	
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomDistanceMin, ZoomDistanceMax);

	UpdateCameraPitch();
}

void ARTSCamera::SelectTarget()
{
	FHitResult Hit = LineTraceToMousePos(ECC_WorldDynamic);

	if (SelectedBuildable && SelectedBuildable->IsBeingCreated())
	{
		BuildStructure();
		return;
	}

	if (Hit.GetActor() && Hit.GetActor()->Implements<UBuildingInterface>())
	{
		switch (CurrentRTSTool)
		{
		case SelectTool:
			Execute_Select(Hit.GetActor(), this);
			break;
		case RecycleTool:
			Execute_Recycle(Hit.GetActor(), this);
			break;
		case MoveTool:
			Execute_Move(Hit.GetActor(), this);
			break;
		}		
	}
}

void ARTSCamera::CancelAction()
{
	if (SelectedBuildable && SelectedBuildable->IsBeingCreated())
	{
		SelectedBuildable->Destroy();
		SelectedBuildable = nullptr;
	}

	if (CurrentRTSTool != ERTSTool::SelectTool)
	{
		CurrentRTSTool = SelectTool;
	}
}

void ARTSCamera::EquipRecycleTool()
{
	CurrentRTSTool = ERTSTool::RecycleTool;
}

void ARTSCamera::EquipMoveTool()
{
	CurrentRTSTool = ERTSTool::MoveTool;
}

void ARTSCamera::BuildStructure()
{
	if (!GetSelectedStructure()->IsBuildingPermitted()) return;
	
	ABuildable* NewStructure = GetWorld()->SpawnActor<ABuildable>(SelectedBuildable->GetClass());
	NewStructure->SetActorLocation(SelectedBuildable->GetActorLocation());
	NewStructure->BeginConstruction();
}

void ARTSCamera::EnableSelectTool()
{
	
}

void ARTSCamera::EnableDestroyTool()
{
	
}

void ARTSCamera::EnableMoveTool()
{
	
}

void ARTSCamera::ExitRTSMode()
{
	verify(GetPlayerController()->GetPlayerCharacter())

	CancelAction();

	GetPlayerController()->Possess(GetPlayerController()->GetPlayerCharacter());
}

FHitResult ARTSCamera::LineTraceToMousePos(ECollisionChannel CollisionChannel)
{
	FHitResult Hit;
	
	FVector TraceStart;
	FVector TraceDirection;
	GetPlayerController()->DeprojectMousePositionToWorld(TraceStart, TraceDirection);
	FVector TraceEnd = TraceStart + TraceDirection * 200000.0f;

	FCollisionQueryParams TraceParams;
	TArray<AActor*> ActorsToIgnore;
	GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	if (SelectedBuildable) ActorsToIgnore.Add(SelectedBuildable);

	TraceParams.AddIgnoredActors(ActorsToIgnore);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, CollisionChannel, TraceParams);
	
	return Hit;
}

void ARTSCamera::SelectStructureBlueprint(TSubclassOf<ABuildable> NewBlueprint)
{
	CancelAction();
	SelectedBuildable = GetWorld()->SpawnActor<ABuildable>(NewBlueprint);
}

void ARTSCamera::MoveStructureToMousePos()
{
	if (!SelectedBuildable) return;

	FHitResult Hit = LineTraceToMousePos(ECC_GameTraceChannel1);

	FVector NewLocation = Hit.ImpactPoint;
	NewLocation = FVector(FMath::RoundToInt32(NewLocation.X / SnappingSize), FMath::RoundToInt32(NewLocation.Y / SnappingSize), NewLocation.Z);
	NewLocation = FVector(NewLocation.X * SnappingSize, NewLocation.Y * SnappingSize, NewLocation.Z);
	SelectedBuildable->SetActorLocation(NewLocation);
}

// Called every frame
void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SelectedBuildable && SelectedBuildable->IsBeingCreated())
	{
		MoveStructureToMousePos();
	}

	if (SpringArm->TargetArmLength != ZoomDistanceTarget)
	{
		UpdateZoom();
	}

	FString EquippedToolString = "Equipped Tool: ";
	switch (CurrentRTSTool)
	{
	case SelectTool:
		EquippedToolString.Append("Select Tool");
		break;
	case RecycleTool:
		EquippedToolString.Append("Recycle Tool");
		break;
	case MoveTool:
		EquippedToolString.Append("Move Tool");
		break;
	}
	GEngine->AddOnScreenDebugMessage(929, 1.0f, FColor::White, EquippedToolString);

	// FString MetalString = "Metal: ";
	// MetalString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Metal));
	// MetalString.Append(" / ");
	// MetalString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::Metal));
	// GEngine->AddOnScreenDebugMessage(930, 1.0f, FColor::White, MetalString);
	//
	// FString AlienMaterialString = "Alien Material: ";
	// AlienMaterialString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::AlienMaterial));
	// AlienMaterialString.Append(" / ");
	// AlienMaterialString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::AlienMaterial));
	// GEngine->AddOnScreenDebugMessage(931, 1.0f, FColor::White, AlienMaterialString);
	//
	// FString FoodString = "Food: ";
	// FoodString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Food));
	// FoodString.Append(" / ");
	// FoodString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::Food));
	// GEngine->AddOnScreenDebugMessage(932, 1.0f, FColor::White, FoodString);
	//
	// FString PowerString = "Power: ";
	// PowerString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Power));
	// PowerString.Append(" / ");
	// PowerString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::Power));
	// GEngine->AddOnScreenDebugMessage(933, 1.0f, FColor::White, PowerString);
	//
	// FString WorkersString = "Workers: ";
	// WorkersString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Workers));
	// WorkersString.Append(" / ");
	// WorkersString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::Workers));
	// GEngine->AddOnScreenDebugMessage(934, 1.0f, FColor::White, WorkersString);
	//
	// FString ScientistsString = "Scientists: ";
	// ScientistsString.AppendInt(GetStrategyGameState()->GetResourceAmount(EResourceType::Scientists));
	// ScientistsString.Append(" / ");
	// ScientistsString.AppendInt(GetStrategyGameState()->GetResourceCapacity(EResourceType::Scientists));
	// GEngine->AddOnScreenDebugMessage(935, 1.0f, FColor::White, ScientistsString);
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

