// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SSwatchTab.h"

#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "SNewSwatchWindow.h"
#include "Asset/DigumWorldSwatch.h"
#include "Objects/DigumWorldEditorSwatch.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSwatchTab::OnNewSwatchClicked()
{
	UDigumWorldEditorSwatch* SwatchObject = NewObject<UDigumWorldEditorSwatch>();
	TSharedRef<SWindow> NewSwatchWindow = SNew(SWindow)
		.Title(FText::FromString("New Swatch"))
		.ClientSize(FVector2D(400, 100));

	TSharedPtr<FDigumWorldEditorToolkit> Toolkit = ToolkitPtr.Pin();
	NewSwatchWindow->SetContent(
		SNew(SBox)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SNewSwatchWindow, Toolkit)
				.Swatch(SwatchObject)
			]
		]
	);
	
	FSlateApplication::Get().AddWindow(NewSwatchWindow);
	return FReply::Handled();
}


void SSwatchTab::Construct(const FArguments& InArgs,  TSharedPtr<FDigumWorldEditorToolkit>& InToolkitPtr)
{
	ToolkitPtr = InToolkitPtr;
	AssetBeingEditedAttribute = InArgs._AssetBeingEdited;
	_Container = SNew(SOverlay);
	
	if(AssetBeingEditedAttribute.Get() != nullptr)
	{
		AssetBeingEditedAttribute.Get()->OnDigumWorldAssetUpdated.AddSP(this, &SSwatchTab::Refresh);
	}
	
	ChildSlot
	[
		_Container.ToSharedRef()
	];

	DrawTab();
}

void SSwatchTab::DrawTab()
{
	_Container->ClearChildren();
	TSharedPtr<SGridPanel> GridPanel = SNew(SGridPanel);

	int32 Width = 8;
	int32 Index = 0;
	_Container->AddSlot()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			GridPanel.ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
			.Text(FText::FromString("New Swatch"))
			.OnClicked(this, &SSwatchTab::OnNewSwatchClicked)
		]
	];

	for(auto Swatch : AssetBeingEditedAttribute.Get()->GetSwatches())
	{
		UDigumWorldSwatch* Asset = Swatch.SoftSwatchAsset.LoadSynchronous();
		if(Asset)
		{
			int32 X = Index % Width;
			int32 Y = Index / Width;
			UMaterialInterface* Material = Asset->EditorMaterial;
			if(Material)
			{
				FSlateMaterialBrush* Brush = new FSlateMaterialBrush(*Material, FVector2D(50.0f, 50.0f));

				if(Brush)
				{
					GridPanel->AddSlot(X, Y)
					[
						SNew(SImage)
						.Image(Brush)
					];
				}

				Index++;
			}
		}
	}
}

void SSwatchTab::Refresh()
{
	DrawTab();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
