// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Building/PowerLine.h"

#include "Components/ArrowComponent.h"


// Sets default values
APowerLine::APowerLine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(StaticMesh);
	Sphere->SetHiddenInGame(false);
	Sphere->SetCollisionProfileName("PowerTrigger");
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnPowerOverlapBegin);
	Sphere->OnComponentEndOverlap.AddUniqueDynamic(this, &ThisClass::OnPowerOverlapEnd);

	PowerLineArrow = CreateDefaultSubobject<UArrowComponent>("Power Line Pos");
	PowerLineArrow->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void APowerLine::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerLine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void APowerLine::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void APowerLine::OnPowerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UPowerInterface>())
	{
		Execute_ConnectPower(OtherActor, this);
	}
}

void APowerLine::OnPowerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->Implements<UPowerInterface>())
	{
		Execute_DisconnectPower(OtherActor, this);
	}
}

bool APowerLine::ConnectPower_Implementation(APowerLine* PowerLine)
{
	SetPowerFeeder(PowerLine);
	
	return true;
}

bool APowerLine::DisconnectPower_Implementation(APowerLine* PowerLine)
{
	ClearPowerFeeder();
	
	return true;
}

bool APowerLine::GetIsConnectedToPowerSource_Implementation()
{
	return IsConnectedToPower();
}

// Called every frame
void APowerLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PowerTarget)
	{
		FVector StartPos = PowerLineArrow->GetComponentLocation();
		FVector EndPos = PowerTarget->PowerLineArrow->GetComponentLocation();

		if (IsConnectedToPower())
		{
			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Yellow, false, -1, 0, 50.0f);
		}
		else
		{
			DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::White, false, -1, 0, 50.0f);
		}		
	}
}

