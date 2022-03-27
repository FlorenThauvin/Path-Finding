// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathFindingGameMode.h"
#include "PathFindingPlayerController.h"
#include "PathFindingCharacter.h"
#include "UObject/ConstructorHelpers.h"

APathFindingGameMode::APathFindingGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = APathFindingPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}