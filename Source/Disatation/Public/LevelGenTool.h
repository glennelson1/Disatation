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
	~ULevelGenTool();
	UPROPERTY(EditAnywhere, Category = "Grid")
	TArray<TSubclassOf<AActor>> CellClasses;
};
