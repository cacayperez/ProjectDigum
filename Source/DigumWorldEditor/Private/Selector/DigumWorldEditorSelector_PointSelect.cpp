// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSelector_PointSelect.h"

TSharedPtr<SWidget> UDigumWorldEditorSelector_PointSelect::CreateSelectorButtonWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Point Select"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectSelectorDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}

void UDigumWorldEditorSelector_PointSelect::BeginSelection()
{
	Super::BeginSelection();
}

void UDigumWorldEditorSelector_PointSelect::EndSelection()
{
	Super::EndSelection();
}

void UDigumWorldEditorSelector_PointSelect::AddSelection(FDigumWorldAssetCoordinate Coordinate)
{
	Selection.AddCoordinate(Coordinate);
	OnSetSelectionDelegate.Broadcast(Coordinate);
	EndSelection();
}

void UDigumWorldEditorSelector_PointSelect::SelectionGeometry(const FGeometry& AllottedGeometry,
	FSlateWindowElementList& OutDrawElements, int32 NewLayerId)
{
	FVector2D CursporPos = FSlateApplication::Get().GetCursorPos();
	FVector2D LocalCursorPos = AllottedGeometry.AbsoluteToLocal(CursporPos);

	FGeometry SquareGeometry = AllottedGeometry.MakeChild(FVector2D(20, 20), FSlateLayoutTransform(LocalCursorPos));
	
	FSlateDrawElement::MakeBox(
					OutDrawElements,
					NewLayerId,
					SquareGeometry.ToPaintGeometry(),
					FCoreStyle::Get().GetBrush(TEXT("WhiteBrush")), // Using a white brush as base
					ESlateDrawEffect::None,
					FLinearColor::Red);
}

