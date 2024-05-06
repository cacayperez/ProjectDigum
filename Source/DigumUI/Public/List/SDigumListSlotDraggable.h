// Copyright Side C Studios Corporation, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SDigumListSlot.h"
#include "Widgets/SCompoundWidget.h"



/**
 * 
 */
class DIGUMUI_API SDigumListSlotDraggable : public SDigumListSlot
{
public:
	
	virtual void OnDrawWidget() override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	

protected:
	bool bCanDrag = false;
	bool bIsDragging = false;
	FVector2D Position;	

	DECLARE_DELEGATE_OneParam(FOnMouseDragStart, const FVector2D&);
	DECLARE_DELEGATE_OneParam(FOnMouseDragStop, const FVector2D&);
	DECLARE_DELEGATE_OneParam(FOnMouseDragMove, const FVector2D&);

public:
	FOnMouseDragStart OnMouseDragStartDelegate;
	FOnMouseDragStop OnMouseDragStopDelegate;
	FOnMouseDragMove OnMouseDragMoveDelegate;
};
