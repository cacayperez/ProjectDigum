// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMUI_API SDigumListSlot : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumListSlot){}
	SLATE_ATTRIBUTE(UObject*, Item)
	SLATE_ATTRIBUTE(float, Height)
	SLATE_ATTRIBUTE(float, Width)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	TAttribute<UObject*> ItemAttribute;
	TAttribute<float> WidthAttribute;
	TAttribute<float> HeightAttribute;

	virtual void DrawWidget();
	virtual void OnDrawWidget();
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
};
