// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasSelectionLayer.h"
#include "DigumWorldEditorToolkit.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasSelectionLayer::Construct(const FArguments& InArgs)
{
	ToolkitPtr = InArgs._Toolkit.Get();
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
	SetVisibility(EVisibility::HitTestInvisible);
}

int32 SCanvasSelectionLayer::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 NewLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);

	if(ToolkitPtr.IsValid())
	{
		ToolkitPtr.Pin()->SelectionGeometry(AllottedGeometry, OutDrawElements, NewLayerId);
	}
	return NewLayerId;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
