using UnrealBuildTool;

public class DigumWorldEditor : ModuleRules
{
    public DigumWorldEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "KismetWidgets",
                "DigumWorld"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "Slate",
                "SlateCore", 
                "InputCore",
                "KismetWidgets", 
                "AssetTools",
                "DigumWorld",
                "EditorStyle",
                "AppFramework"
            }
        );
    }
}