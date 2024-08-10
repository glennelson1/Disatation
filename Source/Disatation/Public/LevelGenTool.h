// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "LevelGenTool.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class DISATATION_API ULevelGenTool : public UEditorUtilityWidget
{
	GENERATED_BODY()
	ULevelGenTool();



protected:
	

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
	

	TMap<int32, float> GetPlaystyleWeightsFromGrid(const FVector2D& Point);

	
	
	int32 GetDifficultyFromPlaystylePosition(const FVector2D& Point);

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
	TMap<int32, float> BaseGen;
	UFUNCTION(BlueprintCallable)
	void DeleteGrid();
	TArray<AActor*> Cellref;
	UFUNCTION(BlueprintCallable)
	void SpawnGrid();
	
	int Explorer, Killer;
	int m_LengthLeft;

	
	bool m_isStart;
	bool m_isEnd;
	int m_EndPos;
	void SpawnSection(int SectNum, int PlayStyle);

	int32  SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities);
	void DetermineProbability();
	void NormalizeProbabilities(TMap<int32, float>& Probabilities);

	TArray<TSubclassOf<AActor>> GetCellArrayForPlaystyle(int32 Playstyle);
	int32 SelectPlaystyleBasedOnWeight(const TMap<int32, float>& Weights);

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	

	
	void SaveIntToFile(int32 Value);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Vector")
	FVector2D ReceivedVector;
	
	int32 AchieversCount;
	int32 KillersCount;
	int32 ExplorersCount;
	int32 SocializersCount;
};
