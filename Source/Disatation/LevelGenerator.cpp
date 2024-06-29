// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGenerator.h"

// Sets default values
ALevelGenerator::ALevelGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SectionProbabilities.Add(0, 10.0f); // Empty
	SectionProbabilities.Add(1, 10.0f); // Pipes
	SectionProbabilities.Add(2, 10.0f); // Stairs
	SectionProbabilities.Add(3, 10.0f); // Single Block
	SectionProbabilities.Add(4, 10.0f); // One Platform
	SectionProbabilities.Add(5, 0.0f); // Small Platforms
	SectionProbabilities.Add(6, 0.0f); // Large Platforms

	SectionProbabilities = {
		{0, 10.0f}, // Empty
		{1, 10.0f}, // Pipes
		{2, 10.0f}, // Stairs
		{3, 10.0f}, // Single Block
		{4, 10.0f}, // One Platform
		{5, 0.0f},  // Small Platforms
		{6, 0.0f}   // Large Platforms
	};

	// Initialize playstyle weights
	PlaystyleWeights = {
		{0, 25.0f}, // Achievers
		{1, 25.0f}, // Explorers
		{2, 25.0f}, // Socializers
		{3, 25.0f}  // Killers
	};
}

// Called when the game starts or when spawned
void ALevelGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}



void ALevelGenerator::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	m_loc = 0;
	Cellref.Empty();
}

void ALevelGenerator::SpawnGrid()
{
	DeleteGrid();
	/*Playstyle = 0;
	switch (Playstyle)
	{
	case 0:
		Cell = AchieversGen;
		break;
	case 1:
		Cell = ExplorersGen;
		break;
	case 2:
		Cell = SocializersGen;
		break;
	case 3:
		Cell = KillersGen;
		break;
	default:
		Cell = CellClasses;
		break;
	}*/
	for (int i = 0; i <= 20; i++)
	{
		/*int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
		SpawnSection(SelectedSection);
		m_loc += 10;
		m_PreviousSect = SelectedSection;*/
		int32 SelectedPlaystyle = SelectPlaystyleBasedOnWeight(PlaystyleWeights);
		Cell = GetCellArrayForPlaystyle(SelectedPlaystyle);

		DetermineProbability(); // Ensure probabilities are adjusted before each selection
		int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
		SpawnSection(SelectedSection);
		m_loc += 10;
	}
	
}

int32  ALevelGenerator::SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities)
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

void ALevelGenerator::DetermineProbability()
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

void ALevelGenerator::NormalizeProbabilities(TMap<int32, float>& Probabilities)
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

TArray<TSubclassOf<AActor>> ALevelGenerator::GetCellArrayForPlaystyle(int32 style)
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

void ALevelGenerator::SpawnSection(int SectNum)
{
	FVector SpawnLocation = FVector(m_loc  * 100, 0,100); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(Cell[SectNum], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}
int32 ALevelGenerator::SelectPlaystyleBasedOnWeight(const TMap<int32, float>& Weights)
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

	return 0; // Default playstyle if something goes wrong
}


// Called every frame
void ALevelGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

