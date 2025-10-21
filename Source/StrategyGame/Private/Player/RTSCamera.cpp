// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/RTSCamera.h"

#include "StrategyStatics.h"
#include "Game/StrategyGameInstance.h"
#include "Player/PlayerCharacter.h"
#include "Game/CityDefenseGameMode.h"
#include "Game/TimeSubsystem.h"

// Sets default values
ARTSCamera::ARTSCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(SceneComponent);	
	
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

	UStrategyGameInstance* StrategyGameInstance = GetGameInstance<UStrategyGameInstance>();

	StrategyGameInstance->OnStructureSelected.AddUniqueDynamic(this, &ThisClass::OnStructureSelected);
	StrategyGameInstance->OnStructureDeSelected.AddUniqueDynamic(this, &ThisClass::OnStructureDeSelected);
}

void ARTSCamera::OnStructureSelected(AStructure* InSelectedStructure)
{
	SelectedStructure = InSelectedStructure;
}

void ARTSCamera::OnStructureDeSelected()
{
	SelectedStructure = nullptr;
}

void ARTSCamera::Move(FVector2D MoveInput)
{
	float MoveSpeedMultiplier = 1000.0f;
	FVector MovementVector = MoveInput.Y * GetActorForwardVector().RotateAngleAxis(GetActorRotation().Pitch, GetActorRightVector()) + MoveInput.X * GetActorRightVector();
	MovementVector *= MoveSpeed * MoveSpeedMultiplier * FApp::GetDeltaTime();
	
	// Used to make the camera move faster when it's more zoomed out, and slower when it's more zoomed in.
	MovementVector *= GetZoomDistanceAlpha() + 1.0f;
	
	SetActorLocation(GetActorLocation() + MovementVector);
}

void ARTSCamera::RotateCamera(float Input)
{
	float RotationAmount = Input * CameraRotationSpeed * FApp::GetDeltaTime();

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
	float ZoomValue = Input * ZoomSpeed * ZoomSpeedMultiplier * FApp::GetDeltaTime();
	
	// Used to make the camera zoom faster when it's more zoomed out, and slower when it's more zoomed in.
	ZoomValue *= GetZoomDistanceAlpha() + 1.0f;

	ZoomDistanceTarget = FMath::Clamp(ZoomDistanceTarget + ZoomValue, ZoomDistanceMin, ZoomDistanceMax);
}

void ARTSCamera::UpdateZoom()
{
	SpringArm->TargetArmLength = FMath::InterpEaseInOut(SpringArm->TargetArmLength, ZoomDistanceTarget, FApp::GetDeltaTime() * ZoomEasingSpeed, ZoomEasing);
	
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, ZoomDistanceMin, ZoomDistanceMax);

	UpdateCameraPitch();
}

void ARTSCamera::SelectTarget()
{
	FHitResult Hit = LineTraceToMousePos(ECC_GameTraceChannel2);

	if (SelectedStructure && SelectedStructure->IsBeingPlaced())
	{
		PlaceStructure();
		return;
	}

	if (Hit.GetActor() && Hit.GetActor()->Implements<UStructureInterface>())
	{
		IStructureInterface* StructureInterface = Cast<IStructureInterface>(Hit.GetActor());
		
		switch (CurrentRTSTool)
		{
		case SelectTool:
			StructureInterface->TrySelect(this);
			break;
			
		case RecycleTool:
			StructureInterface->TryRecycle(this);
			break;
			
		default:
			break;
		}		
	}
	else
	{
		DeselectTarget();
	}
}

void ARTSCamera::DeselectTarget()
{
	SelectedStructure = nullptr;
	GetGameInstance<UStrategyGameInstance>()->OnStructureDeSelected.Broadcast();
}

void ARTSCamera::CancelAction()
{
	if (SelectedStructure && SelectedStructure->IsBeingPlaced())
	{
		SelectedStructure->Destroy();
		SelectedStructure = nullptr;
	}

	if (CurrentRTSTool != ERTSTool::SelectTool)
	{
		CurrentRTSTool = SelectTool;
	}
}

void ARTSCamera::PlaceStructure()
{
	SelectedStructure->PlaceBuilding();
}

void ARTSCamera::RotateStructure()
{
	SelectedStructure->SetActorRotation(SelectedStructure->GetActorRotation() + FRotator(0.0f, 90.0f, 0.0f));
}

void ARTSCamera::EquipRecycleTool()
{
	if (SelectedStructure)
	{
		SelectedStructure->Destroy();
		SelectedStructure = nullptr;
	}
	
	CurrentRTSTool = ERTSTool::RecycleTool;
}

void ARTSCamera::ExitRTSMode()
{
	verify(GetPlayerController()->GetPlayerCharacter())

	CancelAction();

	GetGameInstance()->GetSubsystem<UTimeSubsystem>()->SetTimeScaleMultiplier(1.0f);

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
	if (SelectedStructure) ActorsToIgnore.Add(SelectedStructure);

	TraceParams.AddIgnoredActors(ActorsToIgnore);

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, CollisionChannel, TraceParams);
	
	return Hit;
}

void ARTSCamera::SelectStructure(TSubclassOf<AStructure> NewStructure)
{
	CancelAction();
	SelectedStructure = GetWorld()->SpawnActorDeferred<AStructure>(NewStructure, FTransform());
	SelectedStructure->SetStructureState(EStructureState::BeingPlaced);
	SelectedStructure->FinishSpawning(FTransform());
}

void ARTSCamera::MoveStructureToMousePos()
{
	if (!SelectedStructure) return;

	FHitResult Hit = LineTraceToMousePos(ECC_GameTraceChannel1);
	
	FVector NewLocation = SnapVectorToGrid(Hit.ImpactPoint, UStrategyStatics::GetGridSize(this));
	NewLocation += FVector(SelectedStructure->GetSnappingOffset().X, SelectedStructure->GetSnappingOffset().Y, 0.0f);

	SelectedStructure->MoveBuilding(NewLocation);
}

// Called every frame
void ARTSCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Empty spaces to move the debug messages below the in-game UI.
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::White, " ");
	GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::White, " ");
	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::White, " ");

	if (SelectedStructure && SelectedStructure->IsBeingPlaced())
	{
		MoveStructureToMousePos(); 
	}

	if (SpringArm->TargetArmLength != ZoomDistanceTarget)
	{
		UpdateZoom();
	}
}

FVector ARTSCamera::SnapVectorToGrid(FVector InputPos, int32 GridSize)
{
	FVector Output = InputPos;
	Output = FVector(FMath::RoundToInt32(Output.X / GridSize), FMath::RoundToInt32(Output.Y / GridSize), Output.Z);
	Output = FVector(Output.X * GridSize, Output.Y * GridSize, Output.Z);

	return Output;
}

ARTSPlayerController* ARTSCamera::GetPlayerController()
{
	if (RTSPlayerController == nullptr)
	{
		RTSPlayerController = Cast<ARTSPlayerController>(GetController());
	}

	return RTSPlayerController;
}

