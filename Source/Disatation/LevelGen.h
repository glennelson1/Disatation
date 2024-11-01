// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGen.generated.h"

UCLASS()
class DISATATION_API ALevelGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	bool RandomGen;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> CellClasses;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> AchieversGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> ExplorersGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> SocializersGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> KillersGen;

	int Playstyle;
	TArray<TSubclassOf<AActor>> Cell;
	
	UFUNCTION(BlueprintCallable)
	void DeleteGrid();
	TArray<AActor*> Cellref;
	UFUNCTION(BlueprintCallable)
	void SpawnGrid();

	

	
	void SpawnEmptySection();
	void SpawnPipeSection();
	void SpawnBlockSection(int BlockType, int length, int xAxis);
	void SpawnPlatform(int BlockType, int length);
	void SpawnTopPlatform(int length);
	void SpawnUnder();

	UFUNCTION(BlueprintCallable)
	void SaveLevelSeqToFile();
	int32  SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities);
	void DetermineProbability();
	void NormalizeProbabilities(TMap<int32, float>& Probabilities);
	int m_loc;
	TMap<int32, float> SectionProbabilities;

	
	int m_emptySect, m_pipeSect, m_StairsSect, m_SingleBlockSect,m_singlePlat, m_SmallPlatSect, m_LargePlatSect;
	
	void SpawnEnitiy(FVector CellLocation, int base);
	
	UPROPERTY(EditAnywhere, Category = "Enities")
	TArray<TSubclassOf<AActor>> Enemies;
	UPROPERTY(EditAnywhere, Category = "Enities")
	TArray<TSubclassOf<AActor>> Collectables;
	
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> GridSquare;
	
	TArray<AActor*> EnemyArray;
	TArray<AActor*> EnemyInSection;

	int m_GridStart;
	int m_EnityLastSection;
	AActor* NeighbourDown = nullptr;
	AActor* NeighbourRight = nullptr;
	int m_Difficulty;

	bool m_Start;
	bool m_LastWasEnemy;
	
	FString LevelSeq;
	int m_PreviousSect;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
