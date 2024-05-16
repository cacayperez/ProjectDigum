#include "Commands/DigumWorldEditorCommands.h"

#define LOCTEXT_NAMESPACE "DigumWorldEditorCommands"

FDigumWorldEditorCommands::FDigumWorldEditorCommands()
:TCommands<FDigumWorldEditorCommands>("DigumWorldEditorMode",
		NSLOCTEXT("DigumWorldEditorCommands", "DigumWorldEditorMode", "Digum World Editor Mode"),
		NAME_None,
		FAppStyle::GetAppStyleSetName())
{
}

void FDigumWorldEditorCommands::RegisterCommands()

{
	TArray<TSharedPtr<FUICommandInfo>>& ToolCommands = Commands.FindOrAdd("ToolCommands");

	UI_COMMAND(DesignerTool, "World Designer Tool", "Digum Grid Designer Tool", EUserInterfaceActionType::Button, FInputChord());
	ToolCommands.Add(DesignerTool);
}

TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> FDigumWorldEditorCommands::GetCommands()
{
	return FDigumWorldEditorCommands::Get().Commands;
}

#undef LOCTEXT_NAMESPACE
