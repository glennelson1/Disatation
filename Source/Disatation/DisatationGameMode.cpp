// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisatationGameMode.h"
#include "DisatationCharacter.h"
#include "EnemySpawn.h"
#include "EngineUtils.h"
#include "LevelGenTool.h"
#include "DSP/GrainEnvelope.h"
#include "UObject/ConstructorHelpers.h"

ADisatationGameMode::ADisatationGameMode()
{
    m_Difficulty = 0;

    // Set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void ADisatationGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    ReadIntFromFile();
    GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass());
    
    if(m_Difficulty == 0)
    {
        m_Difficulty = 10;
    }

    
    SpawnEnemies();
}





void ADisatationGameMode::SpawnEnemies()
{
    TArray<AActor*> AvailableSpawnPoints = FoundActors; // Copy the array of spawn points

    // Shuffle the array of spawn points to randomize their order
    AvailableSpawnPoints.Sort([](const AActor& A, const AActor& B) {
        return FMath::RandBool();
    });

    // Set to track which spawn points have been used in this spawning cycle
    TSet<AActor*> SpawnedSpawnPoints;

    // Distribute enemies evenly among spawn points
    int NumSpawnedEnemies = 0;
    
        for (AActor* SpawnPoint : AvailableSpawnPoints)
        {
            if (!SpawnedSpawnPoints.Contains(SpawnPoint))
            {
                // Calculate spawn probability for current difficulty
                float SpawnProbability = ShouldSpawnProbability();
                if (FMath::FRand() <= SpawnProbability)
                {
                    FVector SpawnLocation = SpawnPoint->GetActorLocation();
                    FRotator SpawnRotation = SpawnPoint->GetActorRotation();
                    
                    TSubclassOf<AActor> EnemyClass = ChooseEnemyClass();
                    if (EnemyClass)
                    {
                        AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation);
                        if (SpawnedEnemy)
                        {
                            UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s at %s"), *SpawnedEnemy->GetName(), *SpawnLocation.ToString());
                            NumSpawnedEnemies++;
                        }
                    }

                    // Mark this spawn point as used in the current cycle
                    SpawnedSpawnPoints.Add(SpawnPoint);

                    
                }
            }
        

        // Clear the set for the next spawning cycle
        SpawnedSpawnPoints.Empty();
    }
}


float ADisatationGameMode::ShouldSpawnProbability()
{

    // Define spawn probabilities for lowest and highest difficulties
    float LowestDifficultyProbability = 0.05f;  // 1%
    float HighestDifficultyProbability = 0.150f; // 10%

    // Calculate the linearly interpolated spawn probability based on difficulty
    float DifficultyFactor = FMath::Lerp(LowestDifficultyProbability, HighestDifficultyProbability, (m_Difficulty - 1) / 20.0f);

    // Ensure probability is between 0 and 1
    float Probability = FMath::Clamp(DifficultyFactor, 0.0f, 1.0f);

    return Probability;
}

bool ADisatationGameMode::ShouldSpawnEnemy()
{
    int SpawnChance = FMath::RandRange(1, 100);
    return SpawnChance <= ShouldSpawnProbability() * 100; // Higher probability means higher spawn chance
}

TSubclassOf<AActor> ADisatationGameMode::ChooseEnemyClass()
{
    int EnemyTypeChance = FMath::RandRange(1, 100);
    
    if (EnemyTypeChance <= m_Difficulty * 2.5)
    {
        return HardEnemyClass;
    }
    else if (EnemyTypeChance <= m_Difficulty * 5)
    {
        return MediumEnemyClass;
    }
    else
    {
        return EasyEnemyClass;
    }
}

void ADisatationGameMode::ReadIntFromFile()
{
    FString FilePath = FPaths::ProjectDir() + TEXT("/Content/LevelSequences/GameDiffData.txt");
    FString LoadedString;

    if (FFileHelper::LoadFileToString(LoadedString, *FilePath))
    {
        m_Difficulty = FCString::Atoi(*LoadedString);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load value from file."));
    }
}

void ADisatationGameMode::GetAllActorsOfClass(UWorld* World, TSubclassOf<AActor> ActorClass)
{
    if (World)
    {
        FoundActors.Empty();

        for (TActorIterator<AActor> It(World, ActorClass); It; ++It)
        {
            AActor* Actor = *It;
            if (Actor)
            {
                FoundActors.Add(Actor);
            }
        }
    }
}





