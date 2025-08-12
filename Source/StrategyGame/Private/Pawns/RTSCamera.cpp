// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/RTSCamera.h"

#include "Actors/Road.h"
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

	if (CurrentRTSTool == ERTSTool::RoadBuildingTool)
	{
		PlaceRoad();
		return;
	}

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
		RoadStartPos = FVector::ZeroVector;
		RoadEndPos = FVector::ZeroVector;
	}
}

void ARTSCamera::BuildStructure()
{
	if (!GetSelectedStructure()->IsBuildingPermitted()) return;
	
	ABuildable* NewStructure = GetWorld()->SpawnActor<ABuildable>(SelectedBuildable->GetClass());
	NewStructure->SetActorLocation(SelectedBuildable->GetActorLocation());
	NewStructure->BeginConstruction();
}

void ARTSCamera::PlaceRoad()
{
	if (!RoadClass_1x.GetDefaultObject()) return;
	
	if (RoadStartPos == FVector::ZeroVector)
	{
		FVector StartPos = SnapVectorToGrid(LineTraceToMousePos(ECollisionChannel::ECC_GameTraceChannel1).ImpactPoint, SnappingSize);
		RoadStartPos = StartPos;
		return;
	}
	
	if (RoadEndPos == FVector::ZeroVector)
	{
		FVector EndPos = SnapVectorToGrid(LineTraceToMousePos(ECollisionChannel::ECC_GameTraceChannel1).ImpactPoint, SnappingSize);

		float XDistance = FMath::Abs(EndPos.X - RoadStartPos.X);
		float YDistance = FMath::Abs(EndPos.Y - RoadStartPos.Y);

		if (XDistance > YDistance)
		{
			RoadEndPos = FVector(EndPos.X, RoadStartPos.Y, EndPos.Z);
		}
		else 
		{
			RoadEndPos = FVector(RoadStartPos.X, EndPos.Y, EndPos.Z);
		}
	}
	
	TArray<ARoad*> SpawnedRoads;

	// (↓) As an optimization I am dividing the roads into larger chunks so that there are fewer actors on screen (↓).
	float RemainingRoadLength = FVector::Distance(RoadStartPos, RoadEndPos);
	int32 NumberOf10xRoads = 0;
	int32 NumberOf5xRoads = 0;
	int32 NumberOf2xRoads = 0;
	if (RemainingRoadLength > SnappingSize * 10)
	{
		NumberOf10xRoads = FMath::Floor(RemainingRoadLength / (SnappingSize * 10));
		RemainingRoadLength -= NumberOf10xRoads * SnappingSize * 10;
	}
	if (RemainingRoadLength > SnappingSize * 5)
	{
		NumberOf5xRoads = FMath::Floor(RemainingRoadLength / (SnappingSize * 5));
		RemainingRoadLength -= NumberOf5xRoads * SnappingSize * 5;
	}
	if (RemainingRoadLength > SnappingSize * 2)
	{
		NumberOf2xRoads = FMath::Floor(RemainingRoadLength / (SnappingSize * 2));
		RemainingRoadLength -= NumberOf2xRoads * SnappingSize * 2;
	}

	FVector RoadSpawnPos = RoadStartPos;

	// TODO: Need to tidy up all these loops and try to make them more legible.
	
	if (RoadEndPos.X == RoadStartPos.X && RoadEndPos.Y) // Build on Y Axis
	{
		if (RoadEndPos.Y > RoadStartPos.Y)
		{
			for (int i = 0; i < NumberOf10xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_10x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y += SnappingSize * 10;
			}
			for (int i = 0; i < NumberOf5xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_5x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y += SnappingSize * 5;
			}
			for (int i = 0; i < NumberOf2xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_2x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y += SnappingSize * 2;
			}
			
			while (RoadEndPos.Y >= RoadSpawnPos.Y)
            {
                ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_1x);
                NewRoad->SetActorLocation(RoadSpawnPos);
                SpawnedRoads.Add(NewRoad);
    
            	RoadSpawnPos.Y += SnappingSize;
            }
		}
		else
		{
			for (int i = 0; i < NumberOf10xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_10x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y -= SnappingSize * 10;
			}
			for (int i = 0; i < NumberOf5xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_5x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y -= SnappingSize * 5;
			}
			for (int i = 0; i < NumberOf2xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_2x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y -= SnappingSize * 2;
			}
			
			while (RoadEndPos.Y < RoadSpawnPos.Y)
            {
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_1x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.Y -= SnappingSize;
            }
		}
	}
	else if (RoadEndPos.Y == RoadStartPos.Y) // Build on X Axis
	{
		if (RoadEndPos.X > RoadStartPos.X)
		{
			for (int i = 0; i < NumberOf10xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_10x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X += SnappingSize * 10;
			}
			for (int i = 0; i < NumberOf5xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_5x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X += SnappingSize * 5;
			}
			for (int i = 0; i < NumberOf2xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_2x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X += SnappingSize * 2;
			}
			
			while (RoadEndPos.X > RoadSpawnPos.X)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_1x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, 90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X += SnappingSize;
			}
		}
		else
		{
			for (int i = 0; i < NumberOf10xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_10x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X -= SnappingSize * 10;
			}
			for (int i = 0; i < NumberOf5xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_5x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X -= SnappingSize * 5;
			}
			for (int i = 0; i < NumberOf2xRoads; i++)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_2x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X -= SnappingSize * 2;
			}
			
			while (RoadEndPos.X < RoadSpawnPos.X)
			{
				ARoad* NewRoad = GetWorld()->SpawnActor<ARoad>(RoadClass_1x);
				NewRoad->SetActorLocation(RoadSpawnPos);
				NewRoad->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
				SpawnedRoads.Add(NewRoad);
    
				RoadSpawnPos.X -= SnappingSize;
			}
		}
	}

	for (ARoad* Road : SpawnedRoads)
	{
		Road->SetConnectedRoads(SpawnedRoads);
		Road->RemoveConnectedRoad(Road);
	}

	RoadStartPos = FVector::ZeroVector;
	RoadEndPos = FVector::ZeroVector;
}

void ARTSCamera::EquipRecycleTool()
{
	if (SelectedBuildable)
	{
		SelectedBuildable->Destroy();
		SelectedBuildable = nullptr;
	}
	
	CurrentRTSTool = ERTSTool::RecycleTool;
}

void ARTSCamera::EquipRoadBuildingTool()
{
	if (SelectedBuildable)
	{
		SelectedBuildable->Destroy();
		SelectedBuildable = nullptr;
	}
	
	CurrentRTSTool = ERTSTool::RoadBuildingTool;
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
	
	FVector NewLocation = SnapVectorToGrid(Hit.ImpactPoint, SnappingSize);
	
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

	if (CurrentRTSTool == ERTSTool::RoadBuildingTool)
	{
		if (RoadStartPos != FVector::ZeroVector)
		{
			FVector EndPos = SnapVectorToGrid(LineTraceToMousePos(ECollisionChannel::ECC_GameTraceChannel1).ImpactPoint, SnappingSize);

			float XDistance = FMath::Abs(EndPos.X - RoadStartPos.X);
			float YDistance = FMath::Abs(EndPos.Y - RoadStartPos.Y);

			if (XDistance > YDistance)
			{
				RoadEndPos = FVector(EndPos.X, RoadStartPos.Y, EndPos.Z);
				DrawDebugBox(GetWorld(), (RoadStartPos + RoadEndPos) / 2, FVector(XDistance / 2, SnappingSize / 2, 0.1f), FColor::Green);
			}
			else 
			{
				RoadEndPos = FVector(RoadStartPos.X, EndPos.Y, EndPos.Z);
				DrawDebugBox(GetWorld(), (RoadStartPos + RoadEndPos) / 2, FVector(SnappingSize / 2, YDistance / 2, 0.1f), FColor::Green);
			}
		}
		else
		{
			FVector Center = SnapVectorToGrid(LineTraceToMousePos(ECollisionChannel::ECC_GameTraceChannel1).ImpactPoint, SnappingSize);
			DrawDebugSphere(GetWorld(), Center, 64.0f, 8, FColor::Green);
		}
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

AStrategyGameState* ARTSCamera::GetStrategyGameState()
{
	if (StrategyGameState == nullptr)
	{
		StrategyGameState = Cast<AStrategyGameState>(GetWorld()->GetGameState());
	}

	return StrategyGameState;
}

