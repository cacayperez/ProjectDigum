// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SLayerTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLayerTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

TSharedPtr<SWidget> SLayerTab::OnCreateLayerMenu()
{
	TSharedPtr<SHorizontalBox> Widget = SNew(SHorizontalBox);

	Widget->AddSlot()
	[
		SNew(SButton)
		.Text(FText::FromString("Add New Layer"))
		.OnClicked_Lambda([&]()
		{
			AddNewLayer();
			return FReply::Handled();
		})
	];
	
	return Widget;
}

void SLayerTab::OnLayerNameCommited(const FText& Text, ETextCommit::Type Arg, int InIndex)
{
	if(GetAsset() != nullptr)
	{
		FDigumWorldAssetLayer* Layer = GetAsset()->GetLayer(InIndex);
		Layer->LayerName = Text;

		RefreshTab();
	}
}

TSharedPtr<SWidget> SLayerTab::OnCreateLayerList()
{
	TSharedPtr<SVerticalBox> VerticalContainer = SNew(SVerticalBox);
	if(GetAsset() != nullptr)
	{
		TArray<FDigumWorldAssetLayer> Layers = GetAsset()->GetLayers();
		for(int32 i = 0; i < Layers.Num(); i++)
		{
			const int32 LayerIndex = i;
			FDigumWorldAssetLayer Layer = Layers[i];
			FText LayerName = Layer.GetLayerName();
			VerticalContainer->AddSlot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableText)
					.Text(LayerName)
					.OnTextCommitted(this, &SLayerTab::OnLayerNameCommited, LayerIndex)
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(FText::FromString("X"))
					.OnClicked_Lambda([this, LayerIndex]()
					{
						GetAsset()->RemoveLayer(LayerIndex);
						return FReply::Handled();
					})
				]

			];
		}
	}

	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	[
		VerticalContainer.ToSharedRef()
	];
}

void SLayerTab::AddNewLayer()
{
	if(ToolkitPtr.Pin())
	{
		ToolkitPtr.Pin()->AddNewLayer();
		RefreshTab();
	}
}


void SLayerTab::DrawTab()
{
	_Container->ClearChildren();

	_Container->AddSlot()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			OnCreateLayerList().ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.VAlign(VAlign_Bottom)
		[
			OnCreateLayerMenu().ToSharedRef()
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
