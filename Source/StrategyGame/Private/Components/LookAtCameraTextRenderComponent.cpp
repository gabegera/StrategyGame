// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/LookAtCameraTextRenderComponent.h"

#include "Kismet/KismetMathLibrary.h"

ULookAtCameraTextRenderComponent::ULookAtCameraTextRenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULookAtCameraTextRenderComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FVector LookAtLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

	const FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetComponentLocation(), LookAtLocation);
	
	SetWorldRotation(NewRotation);
	
	
}
