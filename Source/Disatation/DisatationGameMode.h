// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelGenTool.h"
#include "GameFramework/GameModeBase.h"
#include "DisatationGameMode.generated.h"

UCLASS(minimalapi)
class ADisatationGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADisatationGameMode();
	
	UFUNCTION(BlueprintCallable, Category="Game Mode")
	


	virtual void BeginPlay() override;
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int m_Difficulty;
};



