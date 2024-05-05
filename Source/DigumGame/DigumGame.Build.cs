// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DigumGame : ModuleRules
{
	public DigumGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		// unreal engine dependencies
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"});
		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// Utilities and Tools
		PublicDependencyModuleNames.AddRange(new string[] { "DeveloperSettings" });
		
		// Digum Modules
		PublicDependencyModuleNames.AddRange(new string[] { "DigumCore", "DigumUI"});
		
		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
