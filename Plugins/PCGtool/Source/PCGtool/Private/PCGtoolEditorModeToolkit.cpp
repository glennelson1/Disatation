// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCGtoolEditorModeToolkit.h"
#include "PCGtoolEditorMode.h"
#include "Engine/Selection.h"

#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "EditorModeManager.h"

#define LOCTEXT_NAMESPACE "PCGtoolEditorModeToolkit"

FPCGtoolEditorModeToolkit::FPCGtoolEditorModeToolkit()
{
}

void FPCGtoolEditorModeToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost, TWeakObjectPtr<UEdMode> InOwningMode)
{
	FModeToolkit::Init(InitToolkitHost, InOwningMode);
}

void FPCGtoolEditorModeToolkit::GetToolPaletteNames(TArray<FName>& PaletteNames) const
{
	PaletteNames.Add(NAME_Default);
}


FName FPCGtoolEditorModeToolkit::GetToolkitFName() const
{
	return FName("PCGtoolEditorMode");
}

FText FPCGtoolEditorModeToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("DisplayName", "PCGtoolEditorMode Toolkit");
}

#undef LOCTEXT_NAMESPACE
