// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SLayerItem.h"
#include "Asset/DigumWorldAsset.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLayerItem::SelectLayer()
{
	OnLayerSelected.ExecuteIfBound(LayerIndexAttribute.Get());
}

void SLayerItem::Construct(const FArguments& InArgs)
{
	LayerAttribute = InArgs._Layer;
	OnSelectWidget.AddSP(this, &SLayerItem::SelectLayer);
	bVisible = LayerAttribute.Get().IsVisible();
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SLayerItem::OnConstruct()
{
	_Container->ClearChildren();

	
	_Container->AddSlot()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			OnCreateVisibilityWidget().ToSharedRef()
		]
		+ SVerticalBox::Slot()
		[
			OnCreateLayerNameWidget().ToSharedRef()
		]
	];
	
}

void SLayerItem::OnVisibilityChanged(ECheckBoxState NewState)
{
	bVisible = NewState == ECheckBoxState::Checked;
	FDigumWorldAssetLayer Layer = LayerAttribute.Get();
	Layer.SetVisibility(bVisible);
	LayerAttribute.Set(Layer);
	
	OnLayerUpdated.ExecuteIfBound(LayerIndexAttribute.Get(), Layer);
}

ECheckBoxState SLayerItem::GetVisibilityState() const
{
	return bVisible ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

TSharedPtr<SWidget> SLayerItem::OnCreateVisibilityWidget()
{
	const bool bIsVisible = LayerAttribute.Get().IsVisible();
	TSharedPtr<SCheckBox> CheckBox =
		SNew(SCheckBox)
		.IsChecked(this, &SLayerItem::GetVisibilityState)
		.OnCheckStateChanged(this, &SLayerItem::OnVisibilityChanged);

	return CheckBox;
}

FText SLayerItem::GetLayerName() const
{
	return LayerAttribute.Get().GetLayerName();
}

void SLayerItem::OnLayerNameCommited(const FText& Text, ETextCommit::Type Arg)
{
	FText LayerName = Text;
	FDigumWorldAssetLayer Layer = LayerAttribute.Get();
	Layer.LayerName = LayerName;
	LayerAttribute.Set(Layer);
	OnLayerUpdated.ExecuteIfBound(LayerIndexAttribute.Get(), Layer);
}

TSharedPtr<SWidget> SLayerItem::OnCreateLayerNameWidget()
{
	TSharedPtr<SEditableText> EditableText =
		SNew(SEditableText)
		.Text(this, &SLayerItem::GetLayerName)
		.OnTextCommitted(this, &SLayerItem::OnLayerNameCommited);

	return EditableText;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
