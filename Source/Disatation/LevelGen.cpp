// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelGen.h"


// Sets default values
ALevelGen::ALevelGen()
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
}

// Called when the game starts or when spawned
void ALevelGen::BeginPlay()
{
	Super::BeginPlay();
	
}
void ALevelGen::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
		
	}
	m_emptySect= 0, m_pipeSect =0, m_StairsSect =0, m_SingleBlockSect =0,m_singlePlat = 0, m_SmallPlatSect = 0;
	m_LargePlatSect = 0;
	m_PreviousSect = 0;

	LevelSeq.Empty();
}
void ALevelGen::SpawnGrid()
{
	DeleteGrid();
	m_loc = 0;
	
	if (RandomGen)
	{
		for (int i = 0; i <= 20; i++)
		{
			  // Call DetermineProbability to adjust the spawn probabilities based on the previous section
			DetermineProbability();

			// Select the next section type based on the adjusted probabilities
			int32 SelectedSection = SelectSectionBasedOnProbability(SectionProbabilities);
			
            // Spawn the selected section
            switch(SelectedSection)
            {
            case 0:
                SpawnEmptySection();
                m_emptySect++;
                
                break;
            case 1: //pipes
                SpawnPipeSection();
                m_pipeSect++;
                
                break;
            case 2: //stairs 
                SpawnBlockSection(3, 6, 100);
                m_StairsSect++;
              
                break;
            case 3: //single block
                SpawnBlockSection(5, 6, 400);
                m_SingleBlockSect++;
                
                break;
            case 4: //one platform
            	SpawnPlatform(4, 5);
                m_singlePlat++;
                
                break;
            case 5: //small platforms
                SpawnTopPlatform(3);
                m_SmallPlatSect++;
            	
                break;
            case 6: //two large platforms
                SpawnTopPlatform(6);
                m_LargePlatSect++;
                
                break;
            // Ensure there's a default case, possibly to handle unexpected values
            default:
                UE_LOG(LogTemp, Warning, TEXT("Invalid section selection"));
                break;
            }
			m_loc += 10; // Assuming each section advances 'm_loc' by 10 units
			LevelSeq += FString::Printf(TEXT("%d,"), SelectedSection);
			m_PreviousSect = SelectedSection;
			
        }
	}
	else
	{
		LevelSeq = "2,4,1,0,3,2,2,3,1,0,4,2,2,3,4,2,2,3,0,4,6,";

		for (int i = 0; i <= 30; i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), LevelSeq[i]);
		
			switch(LevelSeq[i])
			{
			case '0':
				SpawnEmptySection();
				m_loc += 10;
				break;
			case '1':
				SpawnPipeSection();
				m_loc += 10;
				break;
			case '2':
				SpawnBlockSection(3, 6, 100);
				m_loc += 10;
				break;
			case '3':
				SpawnBlockSection(5, 6, 400);
				m_loc += 10;
				break;
			case '4':
				SpawnPlatform(4, 5);
				m_loc += 10;
				break;
			case '5':
				SpawnTopPlatform(3);
				m_loc += 10;
			case '6':
				SpawnTopPlatform(6);
				m_loc += 10;
			
			}
		}
		
	}
}
void ALevelGen::SpawnEmptySection()
{
	int32 RandomInt= FMath::RandRange(0, 10);
	
	if (RandomInt <= 5)
	{
		FVector SpawnLocation = FVector(m_loc  * 100, 0,100); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[6], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
	else
	{
		FVector SpawnLocation = FVector(m_loc * 100, 0,100); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[7], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}

	
}

void ALevelGen::SpawnPipeSection()
{
	SpawnEmptySection();
	
	FVector SpawnLocation = FVector((m_loc + 3) * 100, 0, 100);
	FVector SpawnLocation2 = FVector((m_loc + 9) * 100, 0, 100);
	AActor* NewCell;
		
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[1], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
		
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[2], SpawnLocation2, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
	int32 randint = FMath::RandRange(0, 100);
	if(randint >=80)
	{
		SpawnUnder();
	}
}



void ALevelGen::SpawnBlockSection(int BlockType, int length, int zAxis)
{
	SpawnEmptySection();
	FVector SpawnLocation = FVector((m_loc + length) * 100, 0,zAxis); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[BlockType], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void ALevelGen::SpawnPlatform(int BlockType, int length)
{
	SpawnEmptySection();
	for (int32 X = m_loc; X < m_loc + 3; X++)
	{
		FVector SpawnLocation = FVector((X + length) * 100, 0,400); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[BlockType], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
	
}

void ALevelGen::SpawnTopPlatform(int length)
{
	SpawnEmptySection();
	SpawnPlatform(4, 5);
	for (int32 X = m_loc; X < m_loc + length; X++)
	{
		FVector SpawnLocation = FVector((X + 10) * 100, 0,800); 
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[4], SpawnLocation, FRotator::ZeroRotator);
		Cellref.Add(NewCell);
	}
}

void ALevelGen::SpawnUnder()
{
	FVector SpawnLocation = FVector(m_loc * 100, 0,-800); 
	AActor* NewCell;
	NewCell = GetWorld()->SpawnActor<AActor>(CellClasses[8], SpawnLocation, FRotator::ZeroRotator);
	Cellref.Add(NewCell);
}

void ALevelGen::SaveLevelSeqToFile()
{
	FString SaveFilePath = FPaths::ProjectDir() + TEXT("/Content/LevelSequences/GameLevelData.txt");
	int32 ChunkSize = 50;
	
	FString SaveDirectory = FPaths::GetPath(SaveFilePath);
	IFileManager& FileManager = IFileManager::Get();
	if (!FileManager.DirectoryExists(*SaveDirectory))
	{
		FileManager.MakeDirectory(*SaveDirectory, true);
	}


	for (int32 StartIndex = 0; StartIndex < LevelSeq.Len(); StartIndex += ChunkSize)
	{
		
		FString SubSequence = LevelSeq.Mid(StartIndex, ChunkSize);
        
		
		FString ContentToSave = SubSequence + TEXT("\n");

		
		bool bWasSuccessful = FFileHelper::SaveStringToFile(ContentToSave, *SaveFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);

		
		if (bWasSuccessful)
		{
			UE_LOG(LogTemp, Log, TEXT("Subsequence successfully appended to %s"), *SaveFilePath);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to append Subsequence to %s"), *SaveFilePath);
		}
	}
}



int32  ALevelGen::SelectSectionBasedOnProbability(const TMap<int32, float>& Probabilities)
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

void ALevelGen::DetermineProbability()
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

void ALevelGen::NormalizeProbabilities(TMap<int32, float>& Probabilities)
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
// Called every frame
void ALevelGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

