// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Disatation : ModuleRules
{
	public Disatation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" ,"UMG"});
		PrivateDependencyModuleNames.AddRange(new string[]{"Blutility","Slate", "SlateCore"});
	}
}
