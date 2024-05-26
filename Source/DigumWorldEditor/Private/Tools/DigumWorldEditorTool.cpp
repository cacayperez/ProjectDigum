// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#include "Tools/DigumWorldEditorTool.h"

void UDigumWorldEditorTool::OnActivateTool(const FDigumWorldEditorToolParams& InParams)
{
}

void UDigumWorldEditorTool::ActivateTool(const FDigumWorldEditorToolParams& InParams)
{
	OnActivateTool(InParams);
}

TSharedPtr<SWidget> UDigumWorldEditorTool::CreateToolWidget()
{
	return nullptr;
}

FText UDigumWorldEditorTool::GetToolName() const
{
	return ToolName;
}

void UDigumWorldEditorTool::SetActive(const bool& bInActive)
{
	bIsActive = bInActive;
}

bool UDigumWorldEditorTool::IsActive() const
{
	return bIsActive;
}
