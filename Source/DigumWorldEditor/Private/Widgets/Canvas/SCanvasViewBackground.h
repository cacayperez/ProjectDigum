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
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
protected:
	
};
