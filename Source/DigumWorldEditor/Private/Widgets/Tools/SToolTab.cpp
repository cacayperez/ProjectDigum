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

	TArray<UDigumWorldEditorTool*> Tools = ToolkitPtr.Pin()->GetTools();

	for(int32 i = 0; i < Tools.Num(); i++)
	{
		UDigumWorldEditorTool* Tool = Tools[i];
		bool bIsActive = ToolkitPtr.Pin()->GetActiveToolIndex() == i;
		Tool->SetActive(bIsActive);
		
		VerticalBox->AddSlot()
		.AutoHeight()
		[
			Tool->CreateToolWidget().ToSharedRef()
		];

		Tool->OnSelectToolDelegate.AddLambda([this, i]()
		{
			UE_LOG(LogTemp, Warning, TEXT("Tool %d selected"), i);
			ToolkitPtr.Pin()->SetActiveTool(i);
			RefreshTab();
		});
	}
	
	_Container->AddSlot()
	[
		VerticalBox.ToSharedRef()
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
