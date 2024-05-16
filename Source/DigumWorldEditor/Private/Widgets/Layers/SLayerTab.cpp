// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SLayerTab.h"

#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLayerTab::Construct(const FArguments& InArgs)
{
	AssetBeingEditedAttribute = InArgs._AssetBeingEdited;

	if(AssetBeingEditedAttribute.Get() != nullptr)
	{
		AssetBeingEditedAttribute.Get()->OnDigumWorldAssetUpdated.AddSP(this, &SLayerTab::Refresh);
	}
	
	_Container = SNew(SOverlay);
	ChildSlot
	[
		_Container.ToSharedRef()
	];

	DrawTab();
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
	if(AssetBeingEditedAttribute.Get() != nullptr)
	{
		FDigumWorldAssetLayer* Layer = AssetBeingEditedAttribute.Get()->GetLayer(InIndex);
		Layer->LayerName = Text;

		Refresh();
	}
}

TSharedPtr<SWidget> SLayerTab::OnCreateLayerList()
{
	TSharedPtr<SVerticalBox> VerticalContainer = SNew(SVerticalBox);
	if(AssetBeingEditedAttribute.Get() != nullptr)
	{
		TArray<FDigumWorldAssetLayer> Layers = AssetBeingEditedAttribute.Get()->GetLayers();
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
						AssetBeingEditedAttribute.Get()->RemoveLayer(LayerIndex);
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
	if(AssetBeingEditedAttribute.Get() == nullptr) return;

	AssetBeingEditedAttribute.Get()->AddNewLayer();
}

void SLayerTab::Refresh()
{
	DrawTab();
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
