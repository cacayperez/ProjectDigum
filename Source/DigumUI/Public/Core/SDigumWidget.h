// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class DIGUMUI_API SDigumWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDigumWidget)
		: _HeightOverride(100.0f)
		, _WidthOverride(100.0f)
	{}
	SLATE_ATTRIBUTE(float, HeightOverride)
	SLATE_ATTRIBUTE(float, WidthOverride)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

private:
	void StartDrag(const FVector2D& Position);
	void StopDrag(const FVector2D& Position);
protected:
	mutable FGeometry WidgetGeometry;
	
	TAttribute<float> HeightOverrideAttribute;
	TAttribute<float> WidthOverrideAttribute;

	FVector2D PointerPosition;
	bool bEnableDrag = false;
	bool bCanDrag = false;
	bool bIsDragging = false;
	
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseDragStart, const FVector2D&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseDragStop, const FVector2D&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMouseDragMove, const FVector2D&);

	// Override with height and width attribute
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;

public:
	FOnMouseDragStart OnMouseDragStartDelegate;
	FOnMouseDragStop OnMouseDragStopDelegate;
	FOnMouseDragMove OnMouseDragMoveDelegate;

	void SetEnableDrag(bool EnableDrag) { bEnableDrag = EnableDrag; }
	bool IsDragEnabled() const { return bEnableDrag; }
	FGeometry GetWidgetGeometry() const { return WidgetGeometry; }
	void SetWidgetGeometry(const FGeometry& Geometry) const { WidgetGeometry = Geometry; }
};
