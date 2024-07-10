// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenTool.h"

#include "EngineUtils.h"

ULevelGenTool::ULevelGenTool()
{
	
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
		{0, 0.0f}, 
		{1, 0.0f}, 
		{2, 0.0f}, 
		{3, 0.0f} };
}


ULevelGenTool::~ULevelGenTool()
{
	
}

TMap<int32, float> ULevelGenTool::GetPlaystyleWeightsFromGrid(const FVector2D& Point)
{
	 TMap<int32, float> Weights;

    // Normalize coordinates based on the new grid dimensions
    float Nx = Point.X / 200.0f;
    float Ny = Point.Y / 200.0f;

    // Define the four corner points of the grid in normalized space
    FVector2D TopLeft(0, 0);
    FVector2D TopRight(1, 0);
    FVector2D BottomLeft(0, 1);
    FVector2D BottomRight(1, 1);

    // Calculate distances from the point to each corner
    float DistToTopLeft = FVector2D::Distance(FVector2D(Nx, Ny), TopLeft);
    float DistToTopRight = FVector2D::Distance(FVector2D(Nx, Ny), TopRight);
    float DistToBottomLeft = FVector2D::Distance(FVector2D(Nx, Ny), BottomLeft);
    float DistToBottomRight = FVector2D::Distance(FVector2D(Nx, Ny), BottomRight);

    
    float Power = 2.0f; 
    DistToTopLeft = FMath::Pow(DistToTopLeft, Power);
    DistToTopRight = FMath::Pow(DistToTopRight, Power);
    DistToBottomLeft = FMath::Pow(DistToBottomLeft, Power);
    DistToBottomRight = FMath::Pow(DistToBottomRight, Power);

  
    float TotalDistance = DistToTopLeft + DistToTopRight + DistToBottomLeft + DistToBottomRight;

    // Check for potential division by zero
    if (TotalDistance == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Total distance is zero, which may cause division by zero error."));
        return Weights;
    }

    // Assign weights inversely proportional to amplified distances
    Weights.Add(0, (0.5f - (DistToTopLeft / TotalDistance)) * 100.0f);    // Playstyle 1
    Weights.Add(1, (0.5f  - (DistToTopRight / TotalDistance)) * 100.0f);   // Playstyle 2
    Weights.Add(2, (0.5f  - (DistToBottomLeft / TotalDistance)) * 100.0f); // Playstyle 3
    Weights.Add(3, (0.5f  - (DistToBottomRight / TotalDistance)) * 100.0f);// Playstyle 4

    // Normalize the weights to sum to 100
    float TotalWeight = 0.0f;
    for (const auto& Elem : Weights)
    {
        TotalWeight += Elem.Value;
    }
    for (auto& Elem : Weights)
    {
        Elem.Value = (Elem.Value / TotalWeight) * 100.0f;
    }

    // Log the calculated weights for debugging
    UE_LOG(LogTemp, Warning, TEXT("Playstyle Weights:"));
    for (const auto& Elem : Weights)
    {
        UE_LOG(LogTemp, Warning, TEXT("Playstyle %d: %f"), Elem.Key, Elem.Value);
    }

    return Weights;
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
	// Use the GraphPosition to adjust the weights
	PlaystyleWeights = GetPlaystyleWeightsFromGrid(ReceivedVector);

	for (int i = 0; i <= 20; i++)
	{
		// Select a playstyle based on the updated weights
		int32 SelectedPlaystyle = SelectPlaystyleBasedOnWeight(PlaystyleWeights);
		Cell = GetCellArrayForPlaystyle(SelectedPlaystyle);

		// Ensure probabilities are adjusted before each selection
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
		Elem.Value /= Total; 
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

bool ULevelGenTool::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	const FVector2D DragWindowOffset = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	//const FVector2D DragWindowOffsetResult = DragWindowOffset - DragWidgetResult->DragOffset;


	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


















