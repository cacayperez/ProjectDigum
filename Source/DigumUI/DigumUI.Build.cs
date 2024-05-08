using UnrealBuildTool;

public class DigumUI : ModuleRules
{
    public DigumUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Engine", "Core", "CoreUObject", "Slate", "SlateCore", "InputCore"});
        // Utilities and Tools
        PublicDependencyModuleNames.AddRange(new string[] { "DeveloperSettings" });
        
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Slate", "SlateCore"});
    }
}