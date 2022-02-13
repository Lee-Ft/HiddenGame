// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"

void AFPSGameState::MulticastOnMissionCompleted_Implementation(APawn* Challenger, bool IsMissionSucceed)
{
	//for (FConstPawnIterator it = GetWorld()->GetPawnIterator(); it; it++)
	//{
	//	APawn* Pawn = it->Get();
	//	if (Pawn && Pawn->IsLocallyControlled())
	//	{
	//		Pawn->DisableInput(nullptr);
	//	}

	//}

	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; it++)
	{
		AFPSPlayerController* PC = Cast<AFPSPlayerController>(it->Get());
		if (PC && PC->IsLocalController())
		{
			PC->OnMissionCompleted(Challenger, IsMissionSucceed);

			APawn* Pawn = PC->GetPawn();

			if (Pawn)
			{
				Pawn->DisableInput(PC);
			}
		}
	}
}
