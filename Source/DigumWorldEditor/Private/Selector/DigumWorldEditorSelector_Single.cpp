// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSelector_Single.h"

TSharedPtr<SWidget> UDigumWorldEditorSelector_Single::CreateSelectorButtonWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Single Select"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectSelectorDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}

void UDigumWorldEditorSelector_Single::BeginSelection()
{
	Super::BeginSelection();
}

void UDigumWorldEditorSelector_Single::EndSelection()
{
	Super::EndSelection();
}

void UDigumWorldEditorSelector_Single::AddSelection(FDigumWorldAssetCoordinate Coordinate)
{
	Selection.AddCoordinate(Coordinate);
	OnSetSelectionDelegate.Broadcast(Coordinate);
	EndSelection();
}
