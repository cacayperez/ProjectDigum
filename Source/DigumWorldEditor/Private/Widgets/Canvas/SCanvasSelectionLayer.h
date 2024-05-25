// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasSelectionLayer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCanvasSelectionLayer)
		{
		}

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
