// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SLayerItem.h"
#include "Asset/DigumWorldAsset.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLayerItem::Construct(const FArguments& InArgs)
{
	LayerAttribute = InArgs._Layer;
	bIsActiveLayer = InArgs._bIsActive;
	bVisible = LayerAttribute.Get()->IsVisible();
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SLayerItem::OnConstruct()
{
	_Container->ClearChildren();

	const FLinearColor SelectedColor = FLinearColor::Yellow;
	const FLinearColor ClearColor = FLinearColor(0.1f,0.1f, 0.1f, 1.0f);
	FLinearColor SelectionColor = ClearColor;
	
	if(bIsActiveLayer)
	{
		SelectionColor = SelectedColor;
	}

	FSlateBorderBrush* SelectionBorderBrush = new FSlateBorderBrush(TEXT("Background"), FMargin(2.0f));
	FSlateColor BorderSlateColor = FSlateColor(SelectionColor);
	// Border

	_Container->AddSlot()
	[
		SNew(SBox)
		[
			SNew(SBorder)
			.BorderBackgroundColor(BorderSlateColor)
			.BorderImage(SelectionBorderBrush)
		]
	];
	
	_Container->AddSlot()
	.Padding(20.0f)
	[
		// spacing
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpacer)
			.Size(FVector2D(20.0f, 0.0f))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			OnCreateVisibilityWidget().ToSharedRef()
		]
		+ SHorizontalBox::Slot()
		[
			OnCreateLayerNameWidget().ToSharedRef()
		]
	];
	
}

void SLayerItem::OnVisibilityChanged(ECheckBoxState NewState)
{
	bVisible = NewState == ECheckBoxState::Checked;
	OnSetLayerVisibility.Broadcast(bVisible);
}

ECheckBoxState SLayerItem::GetVisibilityState() const
{
	return bVisible ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}       

TSharedPtr<SWidget> SLayerItem::OnCreateVisibilityWidget()
{
	const bool bIsVisible = LayerAttribute.Get()->IsVisible();
	TSharedPtr<SCheckBox> CheckBox =
		SNew(SCheckBox)
		.IsChecked(this, &SLayerItem::GetVisibilityState)
		.OnCheckStateChanged(this, &SLayerItem::OnVisibilityChanged);

	return CheckBox;
}

FText SLayerItem::GetLayerName() const
{
	return LayerAttribute.Get()->GetLayerName();
}

void SLayerItem::OnLayerNameCommitted(const FText& Text, ETextCommit::Type Arg)
{
	OnSetLayerName.Broadcast(Text);
}

TSharedPtr<SWidget> SLayerItem::OnCreateLayerNameWidget()
{
	FSlateColorBrush* Brush = new FSlateColorBrush(FLinearColor::Black);
	TSharedPtr<SEditableText> EditableText =
		SNew(SEditableText)
		.Text(this, &SLayerItem::GetLayerName)
		.OnTextCommitted(this, &SLayerItem::OnLayerNameCommitted);

	return SNew(SBorder)
	.BorderImage(Brush)
	[
		EditableText.ToSharedRef()
	];
	
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
