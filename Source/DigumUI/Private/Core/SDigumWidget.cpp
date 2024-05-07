// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Core/SDigumWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumWidget::Construct(const FArguments& InArgs)
{
	HeightOverrideAttribute = InArgs._HeightOverride;
	WidthOverrideAttribute = InArgs._WidthOverride;
}

int32 SDigumWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	WidgetGeometry = AllottedGeometry;
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

void SDigumWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bCanDrag = true;
}

void SDigumWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	// Reset Drag values
	bCanDrag = false;
}


FReply SDigumWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition =  MouseEvent.GetScreenSpacePosition();
	if(bEnableDrag)
	{
		if(EKeys::LeftMouseButton == MouseEvent.GetEffectingButton() && bCanDrag)
		{
			StartDrag(MousePosition);
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
	}
	
	return FReply::Unhandled();
}

FReply SDigumWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition =  MouseEvent.GetScreenSpacePosition();
	if(bIsDragging && EKeys::LeftMouseButton == MouseEvent.GetEffectingButton())
	{
		StopDrag(MousePosition);
		return FReply::Handled();
	}

	return FReply::Unhandled().ReleaseMouseCapture();
}

FReply SDigumWidget::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition =  MouseEvent.GetScreenSpacePosition();
	PointerPosition = MousePosition;
	if(bIsDragging)
	{
		OnMouseDragMoveDelegate.Broadcast(MousePosition);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SDigumWidget::StartDrag(const FVector2D& Position)
{
	bIsDragging = true;
	OnMouseDragStartDelegate.Broadcast(Position);
}

void SDigumWidget::StopDrag(const FVector2D& Position)
{
	bIsDragging = false;
	OnMouseDragStopDelegate.Broadcast(Position);
}

FVector2D SDigumWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(WidthOverrideAttribute.Get(), HeightOverrideAttribute.Get()) * LayoutScaleMultiplier;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
