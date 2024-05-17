// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SSwatchTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "SNewSwatchWindow.h"
#include "Asset/DigumWorldSwatch.h"
#include "Objects/DigumWorldEditorSwatch.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FReply SSwatchTab::OnNewSwatchClicked()
{
	if(NewSwatchWindow.IsValid())
	{
		FSlateApplication::Get().AddWindow(NewSwatchWindow.ToSharedRef());
	}
	
	return FReply::Handled();
}


void SSwatchTab::Construct(const FArguments& InArgs,  TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

void SSwatchTab::OnConstruct()
{
	SBaseTab::OnConstruct();
	UDigumWorldEditorSwatch* SwatchObject = NewObject<UDigumWorldEditorSwatch>();
	
	TSharedPtr<FDigumWorldEditorToolkit> Toolkit = ToolkitPtr.Pin();
	
	TSharedPtr<SNewSwatchWindow> WindowContent =
		SNew(SNewSwatchWindow, Toolkit)
		.Swatch(SwatchObject);
	
	WindowContent->OnAddSwatch.BindLambda([&]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Swatch Added"));
		FSlateApplication::Get().RequestDestroyWindow(NewSwatchWindow.ToSharedRef());
	});
	
	NewSwatchWindow = SNew(SWindow)
	.Title(FText::FromString("New Swatch"))
	.ClientSize(FVector2D(400, 400));

	NewSwatchWindow->SetContent(
	SNew(SBox)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			WindowContent.ToSharedRef()
		]
	]
);
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

	for(auto Swatch : ToolkitPtr.Pin()->GetAssetBeingEdited()->GetSwatches())
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
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
