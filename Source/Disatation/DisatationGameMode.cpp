// Copyright Epic Games, Inc. All Rights Reserved.

#include "DisatationGameMode.h"
#include "EnemySpawn.h"
#include "EngineUtils.h"
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
    
    if (m_Difficulty == 0)
    {
        m_Difficulty = 10;
    }
    
    SpawnEnemies();
}

void ADisatationGameMode::SpawnEnemies()
{
    TArray<AActor*> AvailableSpawnPoints = FoundActors; 

  
    AvailableSpawnPoints.Sort([](const AActor& A, const AActor& B) {
        return FMath::RandBool();
    });

    
    TSet<AActor*> SpawnedSpawnPoints;

    
    int NumSpawnedEnemies = 0;
    int EasyEnemiesCount = 0;
    int MediumEnemiesCount = 0;
    int HardEnemiesCount = 0;

    for (AActor* SpawnPoint : AvailableSpawnPoints)
    {
        if (!SpawnedSpawnPoints.Contains(SpawnPoint))
        {
            
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
                        NumSpawnedEnemies++;

                        if (EnemyClass == EasyEnemyClass)
                        {
                            EasyEnemiesCount++;
                        }
                        else if (EnemyClass == MediumEnemyClass)
                        {
                            MediumEnemiesCount++;
                        }
                        else if (EnemyClass == HardEnemyClass)
                        {
                            HardEnemiesCount++;
                        }

                        UE_LOG(LogTemp, Log, TEXT("Spawned Enemy: %s at %s"), *SpawnedEnemy->GetName(), *SpawnLocation.ToString());
                    }
                }

                
                SpawnedSpawnPoints.Add(SpawnPoint);
            }
        }
    }

    
    SpawnedSpawnPoints.Empty();

    // Log the summary of spawned enemies
    UE_LOG(LogTemp, Log, TEXT("Difficulty: %d"), m_Difficulty);
    UE_LOG(LogTemp, Log, TEXT("Total Enemies Spawned: %d"), NumSpawnedEnemies);
    UE_LOG(LogTemp, Log, TEXT("Easy Enemies: %d, Medium Enemies: %d, Hard Enemies: %d"), EasyEnemiesCount, MediumEnemiesCount, HardEnemiesCount);
}

float ADisatationGameMode::ShouldSpawnProbability()
{
 
    float LowestDifficultyProbability = 0.10f;  
    float HighestDifficultyProbability = 0.30f;
    
    float DifficultyFactor = FMath::Lerp(LowestDifficultyProbability, HighestDifficultyProbability, (m_Difficulty - 1) / 20.0f);
    float Probability = FMath::Clamp(DifficultyFactor, 0.0f, 1.0f);

    return Probability;
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





