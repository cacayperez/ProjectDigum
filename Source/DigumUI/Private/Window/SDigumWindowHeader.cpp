// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindowHeader.h"
#include "Rendering/DrawElements.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumWindowHeader::Construct(const FArguments& InArgs)
{
	
	ChildSlot
	[
		SNew(STextBlock)
		.Text(FText::FromString("Header"))
	];
	
}

FReply SDigumWindowHeader::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FVector2D MousePosition = MouseEvent.GetScreenSpacePosition();
	if(!MyGeometry.IsUnderLocation(MousePosition))
	{
		return FReply::Unhandled();
	}
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bCanDrag)
	{
		bIsDragging = true;
		OnMouseDragStartDelegate.ExecuteIfBound(MousePosition);
		return FReply::Handled().CaptureMouse(SharedThis(this));
	}
	return FReply::Unhandled();
}

FReply SDigumWindowHeader::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
	{
		bIsDragging = false;
		OnMouseDragStopDelegate.ExecuteIfBound(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled().ReleaseMouseCapture();
		
	}
	return FReply::Unhandled();
}


FReply SDigumWindowHeader::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D Size = ChildSlot.GetWidget()->GetDesiredSize();
	const FVector2D LocalClickPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	if (bIsDragging && this->HasMouseCapture())
	{
		OnMouseDragMoveDelegate.ExecuteIfBound(MouseEvent.GetScreenSpacePosition());
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void SDigumWindowHeader::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bCanDrag = true;
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);
}

void SDigumWindowHeader::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	bCanDrag = false;
	SCompoundWidget::OnMouseLeave(MouseEvent);
}




END_SLATE_FUNCTION_BUILD_OPTIMIZATION
