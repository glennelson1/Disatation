// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisatationGameMode.h"
#include "DisatationCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADisatationGameMode::ADisatationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
