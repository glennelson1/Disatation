// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenTool.h"

ULevelGenTool::ULevelGenTool()
{
	SectionProbabilities.Add(0, 10.0f); 
	SectionProbabilities.Add(1, 10.0f); 
	SectionProbabilities.Add(2, 10.0f); 
	SectionProbabilities.Add(3, 10.0f);
	SectionProbabilities.Add(4, 10.0f); 
	SectionProbabilities.Add(5, 0.0f); 
	SectionProbabilities.Add(6, 0.0f); 

	SectionProbabilities = {
		{0, 10.0f}, 
		{1, 10.0f},
		{2, 10.0f},
		{3, 10.0f}, 
		{4, 10.0f},
		{5, 0.0f},  
		{6, 0.0f}   
	};

	// Initialize playstyle weights
	PlaystyleWeights = {
		{0, 25.0f}, // Achievers
		{1, 25.0f}, // Explorers
		{2, 25.0f}, // Socializers
		{3, 25.0f}  // Killers
	};
}

ULevelGenTool::~ULevelGenTool()
{
}

void ULevelGenTool::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	m_loc = 0;
	Cellref.Empty();
}

void ULevelGenTool::SpawnGrid()
{
	DeleteGrid();
	for (int i = 0; i <= 20; i++)
	{
		/*int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
		SpawnSection(SelectedSection);
		m_loc += 10;
		m_PreviousSect = SelectedSection;*/
		int32 SelectedPlaystyle = SelectPlaystyleBasedOnWeight(PlaystyleWeights);
		Cell = GetCellArrayForPlaystyle(SelectedPlaystyle);

		DetermineProbability(); 
		int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
		SpawnSection(SelectedSection);
		m_loc += 10;
	}
}

void ULevelGenTool::SpawnSection(int SectNum)
{
	FVector SpawnLocation = FVector(m_loc  * 100, 0,100); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(Cell[SectNum], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

int32 ULevelGenTool::SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities)
{
	float TotalProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		TotalProbability += Elem.Value;
	}

	
	float RandomPoint = FMath::FRandRange(0.0f, TotalProbability);

	
	float CumulativeProbability = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		CumulativeProbability += Elem.Value;

		
		if (RandomPoint <= CumulativeProbability)
		{
			return Elem.Key; // Return the section type
		}
	}

	return 0;
}

void ULevelGenTool::DetermineProbability()
{
	SectionProbabilities = {
		{0, 10.0f}, // Empty
		{1, 10.0f}, // Pipes
		{2, 10.0f}, // Stairs
		{3, 10.0f}, // Single Block
		{4, 10.0f}, // One Platform
		{5, 0.0f}, // Small Platforms
		{6, 0.0f}  // Large Platforms
	};
	
			
	switch(m_PreviousSect)
	{
	case 0: // Last section was Empty
		SectionProbabilities[1] += 5.0f; 
		SectionProbabilities[2] += 5.0f;
		SectionProbabilities[0] -= 10.0f;
		SectionProbabilities[4] += 10.0f;
		SectionProbabilities[5] += 10.0f;
		SectionProbabilities[6] += 10.0f;
		break;
	case 1: // Last section was Pipes
		SectionProbabilities[0] += 5.0f;
		SectionProbabilities[1] -= 10.0f;
		SectionProbabilities[5] += 5.0f;
		SectionProbabilities[6] += 5.0f;
		SectionProbabilities[3] += 20.0f;
		SectionProbabilities[4] += 10.0f;
		break;
	case 2: // Last section was Stairs
		SectionProbabilities[3] += 10.0f; 
		SectionProbabilities[5] -= 5.0f;
		SectionProbabilities[3] += 5.0f;
		SectionProbabilities[5] += 5.0f;
		SectionProbabilities[4] += 10.0f;
		SectionProbabilities[6] += 5.0f;
		break;
	case 3: // Last section was Single Block
		SectionProbabilities[3] -= 10.0f; 
		SectionProbabilities[4] += 5.0f; 
		SectionProbabilities[0] += 5.0f;
		SectionProbabilities[5] += 5.0f;
		SectionProbabilities[4] += 5.0f;
		SectionProbabilities[6] += 5.0f;
		break;
	case 4: // Last section was One Platform
		SectionProbabilities[5] += 10.0f;
		SectionProbabilities[6] += 10.0f;
		SectionProbabilities[1] -= 5.0f;
		SectionProbabilities[4] -= 10.0f;
		break;
	case 5: // Last section was Small Platforms
		SectionProbabilities[5] -= 10.0f; 
		SectionProbabilities[6] -= 10.0f;
		SectionProbabilities[6] += 10.0f;
		SectionProbabilities[4] += 10.0f;
		
		break;
	case 6: // Last section was Large Platforms
		SectionProbabilities[5] -= 10.0f; 
		SectionProbabilities[6] -= 10.0f;
		SectionProbabilities[2] += 4.0f; 
		SectionProbabilities[0] += 5.0f;
		SectionProbabilities[4] += 10.0f;
		break;
	}
	//UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), Fitness());

	
	NormalizeProbabilities(SectionProbabilities);
}

void ULevelGenTool::NormalizeProbabilities(TMap<int32, float>& Probabilities)
{
	float Total = 0.0f;
	for (const auto& Elem : Probabilities)
	{
		Total += Elem.Value;
	}

	for (auto& Elem : Probabilities)
	{
		Elem.Value = (Elem.Value / Total) * 100.0f; 
	}
}

TArray<TSubclassOf<AActor>> ULevelGenTool::GetCellArrayForPlaystyle(int32 style)
{
	switch (style)
	{
	case 0:
		return AchieversGen;
	case 1:
		return ExplorersGen;
	case 2:
		return SocializersGen;
	case 3:
		return KillersGen;
	default:
		return CellClasses;
	}
}

int32 ULevelGenTool::SelectPlaystyleBasedOnWeight(const TMap<int32, float>& Weights)
{
	float TotalWeight = 0.0f;
	for (const auto& Elem : Weights)
	{
		TotalWeight += Elem.Value;
	}

	float RandomPoint = FMath::FRandRange(0.0f, TotalWeight);

	float CumulativeWeight = 0.0f;
	for (const auto& Elem : Weights)
	{
		CumulativeWeight += Elem.Value;
		if (RandomPoint <= CumulativeWeight)
		{
			return Elem.Key;
		}
	}

	return 0; 
}

bool ULevelGenTool::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	//const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;


	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}