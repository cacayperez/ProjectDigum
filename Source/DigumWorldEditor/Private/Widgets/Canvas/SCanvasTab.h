// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasTab : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCanvasTab)
		{
		}
	SLATE_ATTRIBUTE(class UDigumWorldAsset*, AssetBeingEdited)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
protected:
	TAttribute<UDigumWorldAsset*> AssetBeingEditedAttribute;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	// void RenderGrid();
};
