// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "DigumWorldEditorSelector_BoxSelect.h"

TSharedPtr<SWidget> UDigumWorldEditorSelector_BoxSelect::CreateSelectorButtonWidget()
{
	const FSlateColor SelectedColor = FSlateColor(FLinearColor::Yellow);
	const FSlateColor UnselectedColor = FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	const FSlateColor ButtonColor = IsActive() ? SelectedColor : UnselectedColor;
	return SNew(SBox)
	[
		SNew(SButton)
		.Text(FText::FromString("Box Select"))
		.ButtonColorAndOpacity(ButtonColor)
		.OnClicked_Lambda([this]()
		{
			OnSelectSelectorDelegate.Broadcast();
			return FReply::Handled();
		})
	];
}

void UDigumWorldEditorSelector_BoxSelect::BeginSelection()
{
	Super::BeginSelection();
	bHasBegunSelection = true;
	StartPosition = FSlateApplication::Get().GetCursorPos();
}

void UDigumWorldEditorSelector_BoxSelect::EndSelection()
{
	//
	if (bHasBegunSelection == true && Selection.CoordinateCount() > 0)
	{
	   

		int32 LastIndex = (Selection.CoordinateCount() - 1) > 0 ? (Selection.CoordinateCount() - 1) : 0;
		const int32 StartX = Selection.Coordinates[0].X;
		const int32 StartY = Selection.Coordinates[0].Y;
		const int32 EndX = Selection.Coordinates[LastIndex].X;
		const int32 EndY = Selection.Coordinates[LastIndex].Y;

		// Assuming StartX and StartY are always the top-left corner
		const int32 TopLeftX = FMath::Min(StartX, EndX);
		const int32 TopLeftY = FMath::Min(StartY, EndY);
		const int32 BottomRightX = FMath::Max(StartX, EndX);
		const int32 BottomRightY = FMath::Max(StartY, EndY);

		bHasBegunSelection = false;
		Selection.Clear();
		
	    for (int32 x = TopLeftX; x <= BottomRightX; ++x)
	    {
	        for (int32 y = TopLeftY; y <= BottomRightY; ++y)
	        {
	            int32 CoordinateX = x;
	            int32 CoordinateY = y;
	        	FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(CoordinateX, CoordinateY, SwatchName);
	        	Selection.AddCoordinate(Coordinate);
	        }
	    }
	}


	// FDigumWorldAssetCoordinate Coordinate = FDigumWorldAssetCoordinate(CoordinateX, CoordinateY, SwatchName);
	// Selection.AddCoordinate(Coordinate);
	Super::EndSelection();
}

void UDigumWorldEditorSelector_BoxSelect::AddSelection(FDigumWorldAssetCoordinate Coordinate)
{
	SetSwatchName(Coordinate.SwatchName);
	Selection.AddCoordinate(Coordinate);
	OnSetSelectionDelegate.Broadcast(Coordinate);
	EndPosition = FSlateApplication::Get().GetCursorPos();
}

void UDigumWorldEditorSelector_BoxSelect::SelectionGeometry(const FGeometry& AllottedGeometry,
	FSlateWindowElementList& OutDrawElements, int32 NewLayerId)
{
	Super::SelectionGeometry(AllottedGeometry, OutDrawElements, NewLayerId);

	if(bHasBegunSelection)
	{
		StartPositionLocal = AllottedGeometry.AbsoluteToLocal(StartPosition);
		EndPositionLocal = AllottedGeometry.AbsoluteToLocal(EndPosition);

		// Determine the top-left and bottom-right corners of the selection box
		FVector2D TopLeftCursorPos(FMath::Min(StartPositionLocal.X, EndPositionLocal.X), FMath::Min(StartPositionLocal.Y, EndPositionLocal.Y));
		FVector2D BottomRightCursorPos(FMath::Max(StartPositionLocal.X, EndPositionLocal.X), FMath::Max(StartPositionLocal.Y, EndPositionLocal.Y));

		// Calculate the size of the selection box
		FVector2D BoxSize = BottomRightCursorPos - TopLeftCursorPos;

		// Define the points for the outline of the box
		TArray<FVector2D> BoxPoints;
		BoxPoints.Add(TopLeftCursorPos);                           // Top-left corner
		BoxPoints.Add(FVector2D(BottomRightCursorPos.X, TopLeftCursorPos.Y)); // Top-right corner
		BoxPoints.Add(BottomRightCursorPos);                       // Bottom-right corner
		BoxPoints.Add(FVector2D(TopLeftCursorPos.X, BottomRightCursorPos.Y)); // Bottom-left corner
		BoxPoints.Add(TopLeftCursorPos);                           // Closing the box by returning to the top-left corner

		// Draw the outlined box
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			NewLayerId,
			AllottedGeometry.ToPaintGeometry(),
			BoxPoints,
			ESlateDrawEffect::None,
			FLinearColor::Red,
			true, // Anti-aliasing
			2.0f  // Line thickness
		);
	}
	
}


