// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Buildable.h"
#include "Interfaces/PowerInterface.h"
#include "PowerLine.generated.h"

UCLASS()
class STRATEGYGAME_API APowerLine : public ABuildable, public IPowerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerLine();

protected:

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UArrowComponent* PowerLineArrow;

	// The power line that is supplying this power line.
	UPROPERTY() AActor* PowerFeeder = nullptr;
	// The power line that this power line is sending power to.
	UPROPERTY() APowerLine* PowerTarget = nullptr;
	
	UPROPERTY() bool ConnectedToPower = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION() void OnPowerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION() void OnPowerOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	virtual bool ConnectPower_Implementation(APowerLine* PowerLine) override;
	virtual bool DisconnectPower_Implementation(APowerLine* PowerLine) override;
	virtual bool GetIsConnectedToPowerSource_Implementation() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Power")
	void SetPowerFeeder(AActor* NewPowerFeeder) { if (!PowerFeeder) PowerFeeder = NewPowerFeeder; }

	UFUNCTION(BlueprintCallable, Category="Power")
	void ClearPowerFeeder() { PowerFeeder = nullptr; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Power")
	bool IsConnectedToPower() { return ConnectedToPower; }
};
