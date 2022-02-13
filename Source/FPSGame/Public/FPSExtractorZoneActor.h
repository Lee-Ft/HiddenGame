// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractorZoneActor.generated.h"

class UBoxComponent;
class UDecalComponent;
class USoundBase;
UCLASS()
class FPSGAME_API AFPSExtractorZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSExtractorZoneActor();

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* ObjectMissingSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* MissionCompleteSound;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
