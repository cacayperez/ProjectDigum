using UnrealBuildTool;

public class DigumWorld : ModuleRules
{
    public DigumWorld(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "DigumCore" });
        
        // Utilities and Tools
        PublicDependencyModuleNames.AddRange(new string[] { "DeveloperSettings" });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore"
            }
        );
    }
}