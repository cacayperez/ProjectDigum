#pragma once

class DIGUMWORLDEDITOR_API FDigumWorldEditorCommands : public TCommands<FDigumWorldEditorCommands>
{
public:
	FDigumWorldEditorCommands();

	virtual void RegisterCommands() override;
	static TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> GetCommands();

	TSharedPtr<FUICommandInfo> DesignerTool;

protected:
	TMap<FName, TArray<TSharedPtr<FUICommandInfo>>> Commands;
};
