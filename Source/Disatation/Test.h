// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Test.generated.h"

UCLASS()
class DISATATION_API ATest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATest();
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> GridSquare;

	UPROPERTY(EditAnywhere, Category = "Blocks")
	TArray<TSubclassOf<AActor>> BlockType;

	UPROPERTY(EditAnywhere, Category = "Enities")
	TArray<TSubclassOf<AActor>> Enemies;
	UPROPERTY(EditAnywhere, Category = "Enities")
	TArray<TSubclassOf<AActor>> Collectables;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	int m_Width;
	int m_Height;
	int m_loc;
	int m_GridStart;


	int m_EnityLastSection;
	
	TArray<AActor*> Cellref;
	TArray<AActor*> EnemyArray;
	TArray<AActor*> EnemyInSection;
	
	UFUNCTION(BlueprintCallable)
	void Start();
	
	void DeleteGrid();
	
	AActor* NeighbourDown = nullptr;
	AActor* NeighbourRight = nullptr;
	
	UFUNCTION(BlueprintCallable)
	void SpawnElement(FVector CellLocation);
	void SpawnGrid(int num);
	void SpawnEnemey(FVector CellLocation);
	void SpawnCollectable(FVector CellLocation);

	int CheckSeaction();
	
	//UPROPERTY(EditAnywhere, Category = "Difficulty")
	int m_Difficulty;

	bool m_Start;
	bool m_LastWasEnemy;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
