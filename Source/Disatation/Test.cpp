// Fill out your copyright notice in the Description page of Project Settings.


#include "Test.h"

// Sets default values
ATest::ATest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATest::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATest::Start()
{
	m_Start = true;
	m_LastWasEnemy = false;
	DeleteGrid();
	
	
	for(int i = 0; i <= 140; i += 10)
	{
		SpawnGrid(i);
		
	}
	//EnemyInSection.Empty();
}
//Checks for areas that an enity can be spawned on
void ATest::SpawnGrid(int num)
{
	
	m_Height = 12;
	m_Width = 10;
	FHitResult HitResultDown, HitResultRight;
	FCollisionQueryParams CollisionParams;
	m_GridStart = 0;
	
	m_EnityLastSection = EnemyInSection.Num();
	EnemyInSection.Empty();

	
	
	for(int X = 1; X <= m_Width; X++)
	{
		m_GridStart = (X + num) * 100;
		
		for(int Y = 0; Y <= m_Height; Y++)
		{
			FVector CellLocation = FVector(m_GridStart, 0, Y * 100);
			
			//DrawDebugLine(GetWorld(),CellLocation,CellLocation + FVector(100, 0, 0), FColor::Orange, false, 10.0f );
			bool bHitDown = GetWorld()->LineTraceSingleByChannel(HitResultDown, CellLocation, CellLocation + FVector(0, 0, -100), ECC_Visibility, CollisionParams);
			bool bHitRight = GetWorld()->LineTraceSingleByChannel(HitResultRight, CellLocation, CellLocation + FVector(100, 0, 0), ECC_Visibility, CollisionParams);
			
			NeighbourDown = HitResultDown.GetActor();

			// stops enities being spawned in the floor or in objects
			if(bHitDown)
			{
				if(HitResultDown.GetActor()->IsA(BlockType[0]) || HitResultDown.GetActor()->IsA(BlockType[1]) ||HitResultDown.GetActor()->IsA(BlockType[2]) ||HitResultDown.GetActor()->IsA(BlockType[3])||HitResultDown.GetActor()->IsA(BlockType[4])||HitResultDown.GetActor()->IsA(BlockType[4])||HitResultDown.GetActor()->IsA(BlockType[5]))
				{
					if(bHitRight)
					{
						if(!HitResultRight.GetActor()->IsA(BlockType[0]) &&!HitResultRight.GetActor()->IsA(BlockType[1]) && !HitResultRight.GetActor()->IsA(BlockType[2]) && !HitResultRight.GetActor()->IsA(BlockType[4]))
						{AActor* NewCell;
							NewCell = GetWorld()->SpawnActor<AActor>(GridSquare[0], CellLocation, FRotator::ZeroRotator);
							Cellref.Add(NewCell);
							SpawnElement(CellLocation);
						}
					}
					else
					{
						AActor* NewCell;
						NewCell = GetWorld()->SpawnActor<AActor>(GridSquare[0], CellLocation, FRotator::ZeroRotator);
						Cellref.Add(NewCell);
						SpawnElement(CellLocation);
					}
				}
			}
		}
	}
	m_Start = false;
}

void ATest::DeleteGrid()
{
	for (AActor* actor : Cellref)
	{
		actor->Destroy();
	}
	for (AActor* actor : EnemyArray)
	{
		actor->Destroy();
	}
	Cellref.Empty();
	EnemyArray.Empty();
}


void ATest::SpawnElement(FVector CellLocation)
{
	
	int32 Randomint= FMath::RandRange(0, 100);

	if(Randomint <= 70)
	{
		SpawnEnemey(CellLocation);
	}
	else
	{
		SpawnCollectable(CellLocation);
	}
	
	
	//UE_LOG(LogTemp, Warning, TEXT(" int: %d"), RandomInt);
	
}

void ATest::SpawnEnemey(FVector CellLocation)
{
	int32 RandomInt = FMath::RandRange(0, 100);
	int32 RandEnemyType = FMath::RandRange(0, 10);
	int enemyType;
	if(RandEnemyType <= 7) enemyType = 0;
	else enemyType = 1;
	
	m_Difficulty = CheckSeaction();
	if(!m_LastWasEnemy)
	{
		switch (m_Difficulty) 
		{
		case 0:
			if(RandomInt <= 10)
			{
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(Enemies[enemyType], CellLocation, FRotator::ZeroRotator);
				EnemyArray.Add(NewCell);
				EnemyInSection.Add(NewCell);
				m_LastWasEnemy = true;
			}
			
			break;
		case 1:
			if(RandomInt <= 15)
			{
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(Enemies[enemyType], CellLocation, FRotator::ZeroRotator);
				EnemyArray.Add(NewCell);
				EnemyInSection.Add(NewCell);
				m_LastWasEnemy = true;
			}
			
			break;
		case 2:
			if(RandomInt <= 20)
			{
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(Enemies[enemyType], CellLocation, FRotator::ZeroRotator);
				EnemyArray.Add(NewCell);
				EnemyInSection.Add(NewCell);
				m_LastWasEnemy = true;
			}
			
			break;
		case 3:
			if(RandomInt <= 30)
			{
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(Enemies[enemyType], CellLocation, FRotator::ZeroRotator);
				EnemyArray.Add(NewCell);
				EnemyInSection.Add(NewCell);
				m_LastWasEnemy = true;
			}
			
			break;
		case 4:
			if(RandomInt <= 40)
			{
				AActor* NewCell;
				NewCell = GetWorld()->SpawnActor<AActor>(Enemies[enemyType], CellLocation, FRotator::ZeroRotator);
				EnemyArray.Add(NewCell);
				EnemyInSection.Add(NewCell);
				m_LastWasEnemy = true;
			}
			
			break;
		default: UE_LOG(LogTemp, Warning, TEXT("m_Difficulty = null")); m_LastWasEnemy = false;break;
		}
	}
	else if (m_LastWasEnemy)
	{
		//UE_LOG(LogTemp, Warning, TEXT("1"));
		m_LastWasEnemy = false;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Ememies in seactyion: %d"), m_Difficulty);
}

void ATest::SpawnCollectable(FVector CellLocation)
{
	int32 RandomInt= FMath::RandRange(0, 100);
	if(RandomInt <= 30)
	{
		AActor* NewCell;
		NewCell = GetWorld()->SpawnActor<AActor>(Collectables[0], CellLocation, FRotator::ZeroRotator);
		EnemyArray.Add(NewCell);
		EnemyInSection.Add(NewCell);
			
	}	
}

int ATest::CheckSeaction()
{
	if (m_Start)
	{
		return 0;
	}
	if (m_EnityLastSection == 0)
	{
		return 4;
	}
	if (m_EnityLastSection == 1)
	{
		return 3;
	}
	if (	m_EnityLastSection == 2)
	{
		return 2;
	}
	if (m_EnityLastSection == 3)
	{
		return 1;
	}
	if (m_EnityLastSection == 4)
	{
		return 0;
	}
	else
	{
		return 0;
	}
}

// Called every frame
void ATest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

