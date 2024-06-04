// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "GameMode/DigumGameMenuGameMode.h"

#include "Player/DigumGameMenuPlayerController.h"
#include "Player/Pawn/DigumGameMenuPlayerPawn.h"

ADigumGameMenuGameMode::ADigumGameMenuGameMode()
{
	PlayerControllerClass = ADigumGameMenuPlayerController::StaticClass();
	DefaultPawnClass = ADigumGameMenuPlayerPawn::StaticClass();
	
}
