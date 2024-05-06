// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "List/SDigumListSlotDraggable.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumListSlotDraggable::OnDrawWidget()
{
	SDigumListSlot::OnDrawWidget();
}

FReply SDigumListSlotDraggable::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition = MouseEvent.GetScreenSpacePosition();
	if(!MyGeometry.IsUnderLocation(MousePosition))
	{
		return FReply::Unhandled();
	}
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bCanDrag)
	{
		bIsDragging = true;
		Position = MousePosition;
		OnMouseDragStartDelegate.ExecuteIfBound(MousePosition);
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}
	return FReply::Unhandled();
}

FReply SDigumListSlotDraggable::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
	{
		bIsDragging = false;
		OnMouseDragStopDelegate.ExecuteIfBound(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled().ReleaseMouseCapture();
		
	}
	return FReply::Unhandled();
}

FReply SDigumListSlotDraggable::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition = MouseEvent.GetScreenSpacePosition();
	if (bIsDragging && this->HasMouseCapture())
    {
		Position = MousePosition;
    	OnMouseDragMoveDelegate.ExecuteIfBound(MouseEvent.GetScreenSpacePosition());
    	return FReply::Handled();
    }

    return FReply::Unhandled();
}

void SDigumListSlotDraggable::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bCanDrag = true;
}

void SDigumListSlotDraggable::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bCanDrag = false;
}



int32 SDigumListSlotDraggable::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                                       const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                                       const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	
	FVector2D WidgetSize = FVector2D(WidthAttribute.Get(), HeightAttribute.Get());
	FVector2D NewPosition;
	if(bIsDragging)
		NewPosition = AllottedGeometry.AbsoluteToLocal(Position);
	else
	{
		NewPosition = AllottedGeometry.AbsoluteToLocal(AllottedGeometry.GetAbsolutePosition());
	}
	
	FGeometry NewGeometry = AllottedGeometry.MakeChild(WidgetSize, FSlateLayoutTransform(NewPosition));
	
	// Draw the background
	FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 1,
			NewGeometry.ToPaintGeometry(),
			FCoreStyle::Get().GetBrush("WhiteBrush"),  // Use a default brush
			ESlateDrawEffect::None,
			FLinearColor::Blue
		);
	
	return SDigumListSlot::OnPaint(Args, NewGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
								   bParentEnabled) + 1; 
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

