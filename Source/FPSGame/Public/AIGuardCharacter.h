// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIGuardCharacter.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alert
};

class UPawnSensingComponent;
UCLASS()
class FPSGAME_API AAIGuardCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIGuardCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Component")
	UPawnSensingComponent* SensingComponent;

	FRotator InitialRotation;

	UFUNCTION()
	void ResetRotation();

	FTimerHandle TimerHandle_ResetRotation;

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
	EAIState CurrentAIState;

	UFUNCTION()
		void OnRep_GuardState();

	void SetAIState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAIStateChanged(EAIState NewState);

	void MoveAroundTargetPoint();

	UPROPERTY(EditInstanceOnly, Category = "Patrol")
		bool CanPatrol;

	UPROPERTY(EditAnywhere, Category = "Patrol")
		AActor* StartPoint;
	UPROPERTY(EditAnywhere, Category = "Patrol")
		AActor* EndPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Patrol")
		AActor* CurrentTargetPoint;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void HandleSeenEnemy(APawn* Pawn);

	UFUNCTION()
		void HandleHeardEnemy(APawn* OtherPawn, const FVector& Location, float Volume);

};
