// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCGtoolEditorMode.h"
#include "PCGtoolEditorModeToolkit.h"
#include "EdModeInteractiveToolsContext.h"
#include "InteractiveToolManager.h"
#include "PCGtoolEditorModeCommands.h"


//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
// AddYourTool Step 1 - include the header file for your Tools here
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
#include "Tools/PCGtoolSimpleTool.h"
#include "Tools/PCGtoolInteractiveTool.h"

// step 2: register a ToolBuilder in FPCGtoolEditorMode::Enter() below


#define LOCTEXT_NAMESPACE "PCGtoolEditorMode"

const FEditorModeID UPCGtoolEditorMode::EM_PCGtoolEditorModeId = TEXT("EM_PCGtoolEditorMode");

FString UPCGtoolEditorMode::SimpleToolName = TEXT("PCGtool_ActorInfoTool");
FString UPCGtoolEditorMode::InteractiveToolName = TEXT("PCGtool_MeasureDistanceTool");


UPCGtoolEditorMode::UPCGtoolEditorMode()
{
	FModuleManager::Get().LoadModule("EditorStyle");

	// appearance and icon in the editing mode ribbon can be customized here
	Info = FEditorModeInfo(UPCGtoolEditorMode::EM_PCGtoolEditorModeId,
		LOCTEXT("ModeName", "PCGtool"),
		FSlateIcon(),
		true);
}


UPCGtoolEditorMode::~UPCGtoolEditorMode()
{
}


void UPCGtoolEditorMode::ActorSelectionChangeNotify()
{
}

void UPCGtoolEditorMode::Enter()
{
	UEdMode::Enter();

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// AddYourTool Step 2 - register the ToolBuilders for your Tools here.
	// The string name you pass to the ToolManager is used to select/activate your ToolBuilder later.
	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////// 
	const FPCGtoolEditorModeCommands& SampleToolCommands = FPCGtoolEditorModeCommands::Get();

	RegisterTool(SampleToolCommands.SimpleTool, SimpleToolName, NewObject<UPCGtoolSimpleToolBuilder>(this));
	RegisterTool(SampleToolCommands.InteractiveTool, InteractiveToolName, NewObject<UPCGtoolInteractiveToolBuilder>(this));

	// active tool type is not relevant here, we just set to default
	GetToolManager()->SelectActiveToolType(EToolSide::Left, SimpleToolName);
}

void UPCGtoolEditorMode::CreateToolkit()
{
	Toolkit = MakeShareable(new FPCGtoolEditorModeToolkit);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> UPCGtoolEditorMode::GetModeCommands() const
{
	return FPCGtoolEditorModeCommands::Get().GetCommands();
}

#undef LOCTEXT_NAMESPACE
