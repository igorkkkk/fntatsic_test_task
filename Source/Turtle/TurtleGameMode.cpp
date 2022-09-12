// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurtleGameMode.h"
#include "TurtlePlayerController.h"
#include "TurtlePawn.h"

ATurtleGameMode::ATurtleGameMode()
{
	// no pawn by default
	DefaultPawnClass = ATurtlePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = ATurtlePlayerController::StaticClass();
}
