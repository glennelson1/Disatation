// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class DISATATION_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_Difficulty;

	void ReadIntFromFile();
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
