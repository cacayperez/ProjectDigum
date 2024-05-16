// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasTab.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasTab::Construct(const FArguments& InArgs)
{
	AssetBeingEditedAttribute = InArgs._AssetBeingEdited;
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

int32 SCanvasTab::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
