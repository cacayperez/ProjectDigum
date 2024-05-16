#include "DigumWorldEditor.h"

#include "AssetToolsModule.h"
#include "DigumWorldEditor_AssetTypeActions.h"
#include "Styling/SlateStyleRegistry.h"

#define LOCTEXT_NAMESPACE "FDigumWorldEditorModule"

void FDigumWorldEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	TSharedPtr<IAssetTypeActions> Actions = MakeShareable(new FDigumWorldEditor_AssetTypeActions());
	AssetTools.RegisterAssetTypeActions(Actions.ToSharedRef());
	
	FSlateStyleSet* StyleSet = new FSlateStyleSet("DigumWorldEditorStyle");
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FDigumWorldEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDigumWorldEditorModule, DigumWorldEditor)