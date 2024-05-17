// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasViewBackground : public SGridPanel
{
public:
	SLATE_BEGIN_ARGS(SCanvasViewBackground)
		{
		}
	SLATE_ARGUMENT(float, SquareSize)
	SLATE_ARGUMENT(float, Width);
	SLATE_ARGUMENT(float, Height);
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	// virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
protected:
	mutable FGeometry PanelGeometry = FGeometry();
	float SquareSize = 24.0f;
	float Width = 0.0f;
	float Height = 0.0f;

public:
	FGeometry GetPanelGeometry() const { return PanelGeometry; }
	
};
