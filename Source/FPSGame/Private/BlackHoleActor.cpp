// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHoleActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlackHoleComponent"));
	StaticComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticComp;
	
	DestroyComp = CreateDefaultSubobject<USphereComponent>(TEXT("DestroyComponent"));
	//SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroyComp->SetupAttachment(RootComponent);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("QueryComponent"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetupAttachment(RootComponent);
	
	ForceWeight = 2500;
}

// Called when the game starts or when spawned
void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();

	DestroyComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OverlapInnerSphere);
	
}

// Called every frame
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> OverlappingComponents;
	//SphereComp->GetOverlappingComponents()
	SphereComp->GetOverlappingComponents(OverlappingComponents);
	for (int i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComponents[i];
		if(PrimComp && PrimComp->IsSimulatingPhysics())
		{
			AddForceToComponent(PrimComp);
		}
	}
	
}

void ABlackHoleActor::AddForceToComponent(UPrimitiveComponent* CubeActor)
{
	FVector ForceDirection = SphereComp->GetComponentLocation() - CubeActor->GetComponentLocation();
	CubeActor->AddForce(ForceDirection * ForceWeight);
}


void ABlackHoleActor::OverlapInnerSphere(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to Destroy Inner"));
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}