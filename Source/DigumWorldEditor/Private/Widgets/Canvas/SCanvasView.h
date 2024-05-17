// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SWidgetBase.h"

struct FDigumWorldAssetLayer;
/**
 * 
 */
class DIGUMWORLDEDITOR_API SCanvasView : public SWidgetBase
{
public:
	SLATE_BEGIN_ARGS(SCanvasView)
		{
		}
	SLATE_ARGUMENT(int32, CanvasWidth)
	SLATE_ARGUMENT(int32, CanvasHeight)
	SLATE_ARGUMENT(TArray<FDigumWorldAssetLayer>, Layers)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
protected:
	float SquareSize = 24.0f;
	
	TAttribute<TArray<FDigumWorldAssetLayer>> LayersAttribute;
	TAttribute<int32> CanvasWidthAttribute;
	TAttribute<int32> CanvasHeightAttribute;
	TSharedPtr<SGridPanel> _BackgroundGridPanel;
};
