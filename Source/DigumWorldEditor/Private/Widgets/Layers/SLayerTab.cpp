// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SLayerTab.h"

#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"
#include "SLayerItem.h"
#include "Asset/DigumWorldAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLayerTab::Construct(const FArguments& InArgs, TSharedPtr<FDigumWorldEditorToolkit>& InToolkit)
{
	SBaseTab::Construct(SBaseTab::FArguments(), InToolkit);
}

void SLayerTab::OnSetLayerName(const int32& InLayerIndex, const FText& Text)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SetLayerName(InLayerIndex, Text);
	}
}

void SLayerTab::OnSetLayerVisibility(const int32& InLayerIndex, const bool& bInVisibility)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SetLayerVisibility(InLayerIndex, bInVisibility);
	}
}

void SLayerTab::SwapLayers(const int32& InLayerIndexA, const int32& InLayerIndexB)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SwapLayers(InLayerIndexA, InLayerIndexB);
	}
}

void SLayerTab::OnSetLayerHierarchy(const int32& InLayerIndex, const int32& InHierarchy)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SetLayerHierarchyIndex(InLayerIndex, InHierarchy);
	}
}

TSharedPtr<SWidget> SLayerTab::OnCreateLayerMenu()
{
	TSharedPtr<SHorizontalBox> a = SNew(SHorizontalBox);
	TSharedPtr<SVerticalBox> VerticalContainer = SNew(SVerticalBox);
	
	VerticalContainer->AddSlot()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Move Up"))
			.OnClicked_Lambda([&]()
			{
				if(GetActiveLayerIndex() != INDEX_NONE)
				{
					const int32 IndexA = GetActiveLayerIndex();
					const int32 IndexB = IndexA + 1;
					SwapLayers(IndexA, IndexB);
				}

				return FReply::Handled();
			})
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Move Down"))
			.OnClicked_Lambda([&]()
			{
				if(GetActiveLayerIndex() != INDEX_NONE)
				{
					const int32 IndexA = GetActiveLayerIndex();
					const int32 IndexB = IndexA - 1;
					SwapLayers(IndexA, IndexB);
				}

				return FReply::Handled();
			})
		]
	];

	VerticalContainer->AddSlot()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Add New Layer"))
			.OnClicked_Lambda([&]()
			{
				AddNewLayer();
				return FReply::Handled();
			})
		]
		+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Delete Layer"))
			.OnClicked_Lambda([&]()
			{
				DeleteSelectedLayer();
				return FReply::Handled();
			})
		]

	];
	
	return VerticalContainer;
}

void SLayerTab::OnLayerNameCommitted(const FText& Text, ETextCommit::Type Arg, int InIndex)
{
	if(GetAsset() != nullptr)
	{
		FDigumWorldAssetLayer* Layer = GetAsset()->GetLayer(InIndex);
		Layer->LayerName = Text;

		RefreshTab();
	}
}

int32 SLayerTab::GetActiveLayerIndex() const
{
	if(ToolkitPtr.IsValid())
		return ToolkitPtr.Pin()->GetActiveLayerIndex();

	return INDEX_NONE;
}

TSharedPtr<SWidget> SLayerTab::OnCreateLayerList()
{
	TSharedPtr<SVerticalBox> VerticalContainer = SNew(SVerticalBox);
	TArray<TSharedPtr<SLayerItem>> LayerItems;
	
	if(GetAsset() != nullptr)
	{
		TArray<FDigumWorldAssetLayer> Layers = GetAsset()->GetLayers();
		// for(int32 i = FirstIndex; i >= LastIndex; i--)
		for(int32 i = 0; i < Layers.Num(); i++)
		{
			// const int32 LayerIndex = i;

			const bool bIsActive = (i == GetActiveLayerIndex());
			FDigumWorldAssetLayer* Layer = ToolkitPtr.Pin()->GetAssetBeingEdited()->GetLayer(i);
			FText LayerName = Layer->GetLayerName();

			TSharedPtr<SLayerItem> LayerItem = SNew(SLayerItem)
			.bIsActive(bIsActive)
			.Layer(Layer);

			LayerItem->OnSelectWidget.AddLambda([this, i]()
			{
				OnSelectActiveLayerIndex(i);
			});

			LayerItem->OnSetLayerName.AddLambda([this, i](const FText& InName)
			{
				OnSetLayerName(i, InName);
			});

			LayerItem->OnSetLayerVisibility.AddLambda([this, i](const bool& bInVisibility)
			{
				OnSetLayerVisibility(i, bInVisibility);
			});

			LayerItem->OnSetLayerHierarchy.AddLambda([this, i](const int32& InHierarchy)
			{
				OnSetLayerHierarchy(i, InHierarchy);
			});

			LayerItems.Add(LayerItem);
		}
		
		int32 FirstIndex = Layers.Num() - 1;
		int32 LastIndex = 0;
		
		// Display in reverse Order
		for(int32 i = FirstIndex; i >= LastIndex; i--)
		{
			VerticalContainer->AddSlot()
			.AutoHeight()
			[
				LayerItems[i].ToSharedRef()
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

void SLayerTab::OnSelectActiveLayerIndex(const int32 InIndex)
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SetActiveLayerIndex(InIndex);
		// UE_LOG(LogTemp, Warning, TEXT("Selected Index %i"), InIndex);
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

void SLayerTab::DeleteSelectedLayer()
{
	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->DeleteActiveLayer();
		RefreshTab();
	}
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
