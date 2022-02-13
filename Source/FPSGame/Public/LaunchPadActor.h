// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPadActor.generated.h"

class UParticleSystem;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class FPSGAME_API ALaunchPadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPadActor();
	
	UPROPERTY(EditAnywhere, Category = "Paritcle")
	UParticleSystem* LaunchParticleSystem;
	UPROPERTY(EditAnywhere, Category = "Component")
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, Category = "Component")
		UStaticMeshComponent* ArrowComp;
	UPROPERTY(EditAnywhere, Category = "Component")
	UBoxComponent* LaunchPadComponent;


	UPROPERTY(VisibleAnywhere, Category = "Launch")
		FVector LaunchPower;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void LaunchPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
