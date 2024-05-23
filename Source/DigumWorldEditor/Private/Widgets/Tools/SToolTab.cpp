// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SToolTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "Tools/DigumWorldEditorTool.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SToolTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

void SToolTab::DrawTab()
{
	_Container->ClearChildren();

	TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
	TSharedPtr<SVerticalBox> PaintToolsBox = SNew(SVerticalBox);
	TSharedPtr<SVerticalBox> UtilityToolsBox = SNew(SVerticalBox);

	TArray<UDigumWorldEditorTool*> PaintTools = ToolkitPtr.Pin()->GetPaintTools();
	TArray<UDigumWorldEditorTool*> UtilityTools = ToolkitPtr.Pin()->GetUtilityTools();

	for(int32 i = 0; i < PaintTools.Num(); i++)
	{
		UDigumWorldEditorTool* Tool = PaintTools[i];
		bool bIsActive = ToolkitPtr.Pin()->GetActivePaintToolIndex() == i;
		Tool->SetActive(bIsActive);
		
		PaintToolsBox->AddSlot()
		.AutoHeight()
		[
			Tool->CreateToolWidget().ToSharedRef()
		];

		Tool->OnSelectToolDelegate.AddLambda([this, i]()
		{
			ToolkitPtr.Pin()->SetActiveTool(i);
			RefreshTab();
		});
	}

	for(int32 i = 0; i < UtilityTools.Num(); i++)
	{
		UDigumWorldEditorTool* Tool = UtilityTools[i];
		
		UtilityToolsBox->AddSlot()
		.AutoHeight()
		[
			Tool->CreateToolWidget().ToSharedRef()
		];

		Tool->OnSelectToolDelegate.AddLambda([this, i]()
		{
			ToolkitPtr.Pin()->CallToolAction(i);
			RefreshTab();
		});
	}

	VerticalBox->AddSlot()
	[
		PaintToolsBox.ToSharedRef()
	];

	VerticalBox->AddSlot()
	.AutoHeight()
	[
		UtilityToolsBox.ToSharedRef()
	];
	
	_Container->AddSlot()
	[
		VerticalBox.ToSharedRef()
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
