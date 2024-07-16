// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisatationGameMode.h"
#include "DisatationCharacter.h"
#include "LevelGenTool.h"
#include "DSP/GrainEnvelope.h"
#include "UObject/ConstructorHelpers.h"

ADisatationGameMode::ADisatationGameMode()
{
	//UE_LOG(LogTemp, Warning, TEXT("help"));
	m_Difficulty = 0; 
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	//UE_LOG(LogTemp, Warning, TEXT("m_Difficulty is: %d"), m_Difficulty);
	
}





void ADisatationGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	
}




