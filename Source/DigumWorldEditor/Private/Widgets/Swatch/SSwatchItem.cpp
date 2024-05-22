// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SSwatchItem.h"

#include "SlateMaterialBrush.h"
#include "SlateOptMacros.h"
#include "Asset/DigumWorldSwatchAsset.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SSwatchItem::Construct(const FArguments& InArgs)
{
	AssetAttribute = InArgs._SwatchAsset;
	bIsActiveSwatch = InArgs._bIsActive;
	OnSelectWidget.AddSP(this, &SSwatchItem::OnSelectSwatch);
	OnDeselectWidget.AddSP(this, &SSwatchItem::OnDeselectSwatch);
	
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SSwatchItem::OnConstruct()
{
	_Container->ClearChildren();
	const FLinearColor SelectedColor = FLinearColor::Yellow;
	const FLinearColor ClearColor = FLinearColor(0.1f,0.1f, 0.1f, 1.0f);
	FLinearColor SelectionColor = ClearColor;
	
	if(bIsActiveSwatch)
	{
		SelectionColor = SelectedColor;
	}

	FSlateBorderBrush* SelectionBorderBrush = new FSlateBorderBrush(TEXT("Background"), FMargin(2.0f));
	FSlateColor BorderSlateColor = FSlateColor(SelectionColor);
	// Border
	
	if(AssetAttribute.Get())
	{
		UMaterialInterface* Material = AssetAttribute.Get()->EditorMaterial;
		FSlateMaterialBrush* MaterialBrush = new FSlateMaterialBrush(*Material, FVector2D(40, 40));


		_Container->AddSlot()
		[
			SNew(SBox)
			[
				SNew(SImage)
				.Image(MaterialBrush)
			]
		];
	}

	_Container->AddSlot()
	[
		SNew(SBox)
		[
			SNew(SBorder)
			.BorderBackgroundColor(BorderSlateColor)
			.BorderImage(SelectionBorderBrush)
		]

	];
}

void SSwatchItem::OnSelectSwatch()
{
	UE_LOG(LogTemp, Warning, TEXT("Swatch Selected"));
	RefreshWidget();
}

void SSwatchItem::OnDeselectSwatch()
{
	RefreshWidget();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
