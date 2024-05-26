// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Base/SWidgetBase.h"

class FDigumWorldEditorToolkit;
class UDigumWorldAsset;
class SCanvasViewBackground;
struct FDigumWorldAssetLayer;

struct FDigumCanvasCursorState
{
public:
	bool bHeldDown = false;
};

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
	SLATE_ATTRIBUTE(float, ZoomFactor)
	SLATE_ATTRIBUTE(UDigumWorldAsset*, Asset)
	SLATE_ATTRIBUTE(TSharedPtr<FDigumWorldEditorToolkit>, Toolkit)
	SLATE_END_ARGS()

	virtual ~SCanvasView() override;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	virtual void OnConstruct() override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

private:
	mutable bool bHeldDown = false;
	mutable float ElapsedTime = 0.0f;
protected:
	float Zoom = 1.0f;
	float SquareSize = 24.0f;
	float ScaledSquareSize = SquareSize * Zoom;
	TWeakPtr<FDigumWorldEditorToolkit> ToolkitPtr;
	
	TAttribute<UDigumWorldAsset*> Asset;
	TAttribute<int32> CanvasWidthAttribute;
	TAttribute<int32> CanvasHeightAttribute;
	TSharedPtr<SGridPanel> _BackgroundGridPanel;
	TSharedPtr<SGridPanel> _LayersPanel;
	
	void ProcessScroll(const float InScrollDelta);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSelectCanvasCoordinate, const int32&, const int32&);
	DECLARE_MULTICAST_DELEGATE(FOnBeginSelection);
	DECLARE_MULTICAST_DELEGATE(FOnEndSelection);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetZoomFactor, const float&);
	
public:
	TSharedPtr<FSlateColorBrush> TestBrush;
	FOnSelectCanvasCoordinate OnSelectCanvasCoordinate;
	FOnSetZoomFactor OnSetZoomFactor;
	FOnBeginSelection OnBeginSelection;
	FOnEndSelection OnEndSelection;
	
	void SelectCoordinate(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) const;
	bool IsHeldDown() const;

};
