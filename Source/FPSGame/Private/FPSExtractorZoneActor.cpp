// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractorZoneActor.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AFPSExtractorZoneActor::AFPSExtractorZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->SetBoxExtent(FVector(200.0f));
	BoxComp->SetHiddenInGame(false);
	RootComponent = BoxComp;
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractorZoneActor::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSExtractorZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSExtractorZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSExtractorZoneActor::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* PickUpCharacter = Cast<AFPSCharacter>(OtherActor);
	
	if (PickUpCharacter == nullptr)
	{
		return;
	}
	else
	{
		if (PickUpCharacter->IsPickObjective == true)
		{

			UE_LOG(LogTemp, Warning, TEXT("You finished this mission"));
			AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
			if (GameMode)
			{
				GameMode->MissionCompleted(PickUpCharacter, true);
				UGameplayStatics::PlaySound2D(this, MissionCompleteSound);
			}

		}
		else
		{
			UGameplayStatics::PlaySound2D(this, ObjectMissingSound);
		}
	}
	

}
