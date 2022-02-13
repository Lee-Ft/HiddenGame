// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPadActor.h"
#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ALaunchPadActor::ALaunchPadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComp;

	LaunchPadComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchPadComponent"));
	LaunchPadComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LaunchPadComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	LaunchPadComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LaunchPower = FVector(0, 0, 1000.f);
	
	//LaunchParticleSystem = CreateDefaultSubobject<UParticleSystem>(TEXT("LanchParticleSystem"));
	LaunchPadComponent->SetupAttachment(RootComponent);

	ArrowComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowComp"));
	ArrowComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArrowComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALaunchPadActor::BeginPlay()
{
	Super::BeginPlay();
	LaunchPadComponent->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPadActor::LaunchPlayer);

}

// Called every frame
void ALaunchPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaunchPadActor::LaunchPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//OtherActor->LaunchCharacter(FVector(0, 0, 100.f));
	UE_LOG(LogTemp, Warning, TEXT("Launch Player!"));

	AFPSCharacter* OverlappedActor = Cast<AFPSCharacter>(OtherActor);
	if (OverlappedActor)
	{
		OverlappedActor->LaunchCharacter(LaunchPower, false, false);

		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchParticleSystem, this->GetActorLocation());
	}
	else if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchPower);
	}

}

