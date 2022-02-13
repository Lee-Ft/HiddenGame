 // Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuardCharacter.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Perception/PawnSensingComponent.h"
#include "..\Public\AIGuardCharacter.h"
#include "Net/UnrealNetwork.h"
 // Sets default values
AAIGuardCharacter::AAIGuardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing Component"));
	CurrentAIState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AAIGuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	SensingComponent->OnSeePawn.AddDynamic(this, &AAIGuardCharacter::HandleSeenEnemy);
	SensingComponent->OnHearNoise.AddDynamic(this, &AAIGuardCharacter::HandleHeardEnemy);

	InitialRotation = GetActorRotation();

	if (CanPatrol)
	{
		MoveAroundTargetPoint();
	}

}

// Called every frame
void AAIGuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentTargetPoint)
	{
		float TargetDistance = (GetActorLocation() - CurrentTargetPoint->GetActorLocation()).Size();
		if (TargetDistance < 200.f)
		{
			MoveAroundTargetPoint();
		}
	}


}

void AAIGuardCharacter::HandleSeenEnemy(APawn* Pawn)
{
	if (Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Seen Enemy"));
		DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 20.f, 12, FColor::Red, false, 5.f);
		
		AFPSGameMode* CurrentGameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (CurrentGameMode)
		{
			CurrentGameMode->MissionCompleted(Pawn, false);
		}
	}

	SetAIState(EAIState::Alert);
	AController* CurrentController = GetController();
	if (CurrentController)
	{
		CurrentController->StopMovement();
	}
}

void AAIGuardCharacter::HandleHeardEnemy(APawn* OtherPawn, const FVector& Location, float Volume)
{
	if (CurrentAIState == EAIState::Alert)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 20.f, 12, FColor::Green, false, 5.f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLookAt =  FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0;
	NewLookAt.Roll = 0;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetRotation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetRotation, this, &AAIGuardCharacter::ResetRotation, 3.0f, false);

	SetAIState(EAIState::Suspicious);

	AController* CurrentController = GetController();
	if (CurrentController)
	{
		CurrentController->StopMovement();
	}
}

void AAIGuardCharacter::ResetRotation()
{
	if (CurrentAIState == EAIState::Alert)
	{
		return;
	}
	SetActorRotation(InitialRotation);
	SetAIState(EAIState::Idle);

	if (CanPatrol)
	{
		MoveAroundTargetPoint();
	}
	
}

void AAIGuardCharacter::OnRep_GuardState()
{
	OnAIStateChanged(CurrentAIState);
}

void AAIGuardCharacter::SetAIState(EAIState NewState)
{
	if (NewState == CurrentAIState)
	{
		return;
	}
	CurrentAIState = NewState;
	
	OnRep_GuardState();
	//OnAIStateChanged(NewState);
}

void AAIGuardCharacter::MoveAroundTargetPoint()
{

	if (CurrentTargetPoint == nullptr || CurrentTargetPoint == EndPoint)
	{
		CurrentTargetPoint = StartPoint;
	}
	else if (CurrentTargetPoint == StartPoint)
	{
		CurrentTargetPoint = EndPoint;
	}

	
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentTargetPoint);
}

void AAIGuardCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAIGuardCharacter, CurrentAIState);
}