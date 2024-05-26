// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class FDigumWorldEditorToolkit;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasSelectionLayer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCanvasSelectionLayer)
		{
		}
	SLATE_ATTRIBUTE(TSharedPtr<FDigumWorldEditorToolkit>, Toolkit)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
