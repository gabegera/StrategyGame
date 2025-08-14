// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Pawns/RTSCamera.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetMesh(), "Head");
	FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 20.0f));
	FirstPersonCamera->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &APlayerCharacter::CheckForInteractable, 1.0f / InteractionChecksPerSecond, true);
}

void APlayerCharacter::Move(FVector2D MoveInput)
{
	if (IsSprinting())
	{
		MoveInput.X = FMath::Clamp(MoveInput.X, -SprintStrafeSpeedMultiplier, SprintStrafeSpeedMultiplier);
		MoveInput.Y = FMath::Clamp(MoveInput.Y, -SprintStrafeSpeedMultiplier, MaxSprintSpeed);
	}
	
	AddMovementInput(GetActorForwardVector() * MoveInput.Y);
	AddMovementInput(GetActorRightVector() * MoveInput.X);
}

void APlayerCharacter::Look(FVector2D LookInput)
{
	if (IsSprinting())
	{
		LookInput.X = FMath::Clamp(LookInput.X, -MaximumSprintLookSpeed, MaximumSprintLookSpeed);
	}
	
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSprintSpeed;
}

void APlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void APlayerCharacter::TriggerInteraction()
{
	if (!TargetInteractable) return;

	Cast<IInteractionInterface>(TargetInteractable)->Interact(this);
}

void APlayerCharacter::CheckForInteractable()
{
	FHitResult Hit;
	FVector TraceStart = FirstPersonCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + FirstPersonCamera->GetForwardVector() * InteractionRange;
	bool TraceComplex = false;
	TArray<AActor*> ActorsToIgnore;
	GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	bool IgnoreSelf = true;

	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), TraceStart, TraceEnd, InteractionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		TraceComplex, ActorsToIgnore, EDrawDebugTrace::None, Hit, IgnoreSelf);

	if (Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
	{
		TargetInteractable = Hit.GetActor();
	}
	else
	{
		TargetInteractable = nullptr;
	}
}

void APlayerCharacter::SwitchToRTSCam(ARTSCamera* TargetCamera)
{
	if (!TargetCamera) return;
	
	GetPlayerController()->Possess(TargetCamera);
}

void APlayerCharacter::EnterSeat(AActor* Seat)
{
    SetActorEnableCollision(false);
    SetActorLocation(Seat->GetActorLocation() + Seat->GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
    GetPlayerController()->SetControlRotation(Seat->GetActorRotation());
}

void APlayerCharacter::Exit()
{
	GetPlayerController()->SetControllerMode(EControllerMode::FirstPerson);
	SetControlledTurret(nullptr);
	SetActorEnableCollision(true);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ARTSPlayerController* APlayerCharacter::GetPlayerController()
{
	if (RTSPlayerController == nullptr)
	{
		RTSPlayerController = Cast<ARTSPlayerController>(GetController());
	}

	return RTSPlayerController;
}

