// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"
#include "EquippableItems/EquippableItem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/RTSCamera.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetRootComponent());
	FirstPersonCamera->bUsePawnControlRotation = true;

	GetMesh()->SetupAttachment(FirstPersonCamera);
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultFOV = GetFirstPersonCamera()->FieldOfView;
	MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	CrouchHeight = GetCharacterMovement()->GetCrouchedHalfHeight();
	
	GetWorldTimerManager().SetTimer(InteractionTimer, this, &APlayerCharacter::CheckForInteractable, 1.0f / InteractionChecksPerSecond, true);

	RTSPlayerController = Cast<AStrategyPlayerController>(GetController());
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

void APlayerCharacter::Crouch(bool bClientSimulation)
{
	Super::Crouch(bClientSimulation);
}

void APlayerCharacter::UnCrouch(bool bClientSimulation)
{	
	Super::UnCrouch(bClientSimulation);
}

void APlayerCharacter::Prone()
{
	
}

void APlayerCharacter::UnProne()
{
	
}

void APlayerCharacter::SlowWalk()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxSlowWalkSpeed;
}

void APlayerCharacter::StopSlowWalking()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}

void APlayerCharacter::UseEquippedItem()
{
	Super::UseEquippedItem();
}

void APlayerCharacter::UseEquippedItemSecondary()
{
	Super::UseEquippedItemSecondary();
}

void APlayerCharacter::EquipItem(AEquippableItem* NewItem)
{
	Super::EquipItem(NewItem);
}

void APlayerCharacter::HolsterEquippedItem()
{
	Super::HolsterEquippedItem();
}

void APlayerCharacter::TriggerInteraction()
{
	if (!TargetInteractable) return;

	// If the target interactable is being carried by the player, return.
	if (CharacterInventoryComponent->GetCarriedEquipment().Contains(Cast<AEquippableItem>(TargetInteractable))) return;
	
	Cast<IInteractionInterface>(TargetInteractable)->TryInteract(this);
}

void APlayerCharacter::CheckForInteractable()
{
	TArray<FHitResult> Hits;
	FVector TraceStart = FirstPersonCamera->GetComponentLocation();
	FVector TraceEnd = TraceStart + FirstPersonCamera->GetForwardVector() * InteractionRange;
	bool TraceComplex = false;
	TArray<AActor*> ActorsToIgnore;
	GetAllChildActors(ActorsToIgnore);
	ActorsToIgnore.Add(this);
	bool IgnoreSelf = true;

	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), TraceStart, TraceEnd, InteractionRadius, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		TraceComplex, ActorsToIgnore, EDrawDebugTrace::None, Hits, IgnoreSelf);

	for (FHitResult Hit : Hits)
	{
		if (Hit.GetActor() && Hit.GetActor()->Implements<UInteractionInterface>())
		{
			TargetInteractable = Hit.GetActor();
			return;
		}
	}
	
	TargetInteractable = nullptr;
}

void APlayerCharacter::SwitchToRTSCam(ARTSCamera* TargetCamera)
{
	if (!TargetCamera) return;
	
	GetController()->Possess(TargetCamera);
}

void APlayerCharacter::EnterSeat(AActor* Seat)
{
    SetActorEnableCollision(false);
    SetActorLocation(Seat->GetActorLocation() + Seat->GetActorUpVector() * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
    GetPlayerController()->SetControlRotation(Seat->GetActorRotation());
	FirstPersonCamera->bUsePawnControlRotation = false;
}

void APlayerCharacter::Exit()
{
	GetPlayerController()->SetControllerMode(EControllerMode::FirstPerson);
	SetControlledTurret(nullptr);
	SetActorEnableCollision(true);
	GetMesh()->SetAnimation(nullptr);
	FirstPersonCamera->bUsePawnControlRotation = true;
	GetPlayerController()->SetViewTargetWithBlend(this, 1.0f, VTBlend_EaseInOut, 2);
	
	FInputModeGameOnly InputMode;
	GetPlayerController()->SetInputMode(InputMode);
}

void APlayerCharacter::ResetFOV()
{
	FirstPersonCamera->FieldOfView = DefaultFOV;
}

void APlayerCharacter::OpenObjectiveMenu()
{
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

