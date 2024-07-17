// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"

#include "EngineUtils.h"

// Sets default values
AEnemySpawn::AEnemySpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
	ReadIntFromFile();
	
}

void AEnemySpawn::ReadIntFromFile()
{
	FString FilePath = FPaths::ProjectDir() + TEXT("/Content/LevelSequences/GameDiffData.txt");
	FString LoadedString;


	if (FFileHelper::LoadFileToString(LoadedString, *FilePath))
	{
		
		m_Difficulty = FCString::Atoi(*LoadedString);
		//UE_LOG(LogTemp, Log, TEXT("Loaded value: %d"), m_Difficulty);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load value from file."));
	}
}




// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

