// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Turtle : ModuleRules
{
	public Turtle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
