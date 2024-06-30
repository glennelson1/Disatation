// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelGenerator.generated.h"

UCLASS()
class DISATATION_API ALevelGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> AchieversGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> ExplorersGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> SocializersGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> KillersGen;
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> CellClasses;



	TMap<int32, float> AchieversProbabilities;
	TMap<int32, float> ExplorersProbabilities;
	TMap<int32, float> SocializersProbabilities;
	TMap<int32, float> KillersProbabilities;
	TMap<int32, float> CurrentProbabilities;

	
	int Playstyle;
	TArray<TSubclassOf<AActor>> Cell;
	
	int m_loc;
	int m_PreviousSect;
	TMap<int32, float> SectionProbabilities;
	TMap<int32, float> PlaystyleWeights;
	UFUNCTION(BlueprintCallable)
	void DeleteGrid();
	TArray<AActor*> Cellref;
	UFUNCTION(BlueprintCallable)
	void SpawnGrid();

	void SpawnSection(int SectNum);

	int32  SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities);
	void DetermineProbability();
	void NormalizeProbabilities(TMap<int32, float>& Probabilities);

	TArray<TSubclassOf<AActor>> GetCellArrayForPlaystyle(int32 style);
	int32 SelectPlaystyleBasedOnWeight(const TMap<int32, float>& Weights);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
