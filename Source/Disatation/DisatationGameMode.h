// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenTool.h"
#include "GameFramework/GameModeBase.h"
#include "DisatationGameMode.generated.h"

UCLASS(minimalapi)
class ADisatationGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADisatationGameMode();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_Difficulty;

	void ReadIntFromFile();
	
	bool ShouldSpawnEnemy();
	TSubclassOf<AActor> ChooseEnemyClass();
	
	void GetAllActorsOfClass(UWorld* World, TSubclassOf<AActor> ActorClass);
	TArray<AActor*> FoundActors;

	float ShouldSpawnProbability();

	int CalculateTotalEnemiesToSpawn();

	void SpawnEnemies();

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> EasyEnemyClass;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> MediumEnemyClass;
	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AActor> HardEnemyClass;
};

