// Copyright Epic Games, Inc. All Rights Reserved.

#include "PCGtoolModule.h"
#include "PCGtoolEditorModeCommands.h"

#define LOCTEXT_NAMESPACE "PCGtoolModule"

void FPCGtoolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FPCGtoolEditorModeCommands::Register();
}

void FPCGtoolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPCGtoolEditorModeCommands::Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPCGtoolModule, PCGtoolEditorMode)