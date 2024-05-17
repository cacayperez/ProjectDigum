// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasViewBackground.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasViewBackground::Construct(const FArguments& InArgs)
{
	Width = InArgs._Width;
	Height = InArgs._Height;
	SquareSize = InArgs._SquareSize;
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

// FVector2D SCanvasViewBackground::ComputeDesiredSize(float LayoutScaleMultiplier) const
// {
// 	
// 	return FVector2D(Width, Height);
// }

int32 SCanvasViewBackground::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	PanelGeometry = AllottedGeometry;
	return SGridPanel::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                           bParentEnabled);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
