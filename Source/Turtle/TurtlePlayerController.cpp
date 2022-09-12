// Copyright Epic Games, Inc. All Rights Reserved.

#include "TurtlePlayerController.h"

ATurtlePlayerController::ATurtlePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
