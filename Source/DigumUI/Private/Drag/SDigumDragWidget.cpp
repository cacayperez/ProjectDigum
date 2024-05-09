// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Drag/SDigumDragWidget.h"

#include "SlateOptMacros.h"
#include "Core/SDigumWidgetStack.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SDigumDragWidget::OnConstruct()
{
	SDigumWidget::OnConstruct();

	_Container->AddSlot()
	[
		SNew(STextBlock)
		.Text(FText::FromString("Drag me"))
	];
}

void SDigumDragWidget::OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SDigumWidget::OnTick(AllottedGeometry, InCurrentTime, InDeltaTime);

}

int32 SDigumDragWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const FVector2D& CursorPosition = FSlateApplication::Get().GetCursorPos();
	const FVector2D LocalCoordinates = AllottedGeometry.AbsoluteToLocal(CursorPosition);
	const FVector2D GeometrySize = AllottedGeometry.GetLocalSize();
	
	const FGeometry NewGeometry = AllottedGeometry.MakeChild(GeometrySize, FSlateLayoutTransform( LocalCoordinates));
	return SDigumWidget::OnPaint(Args, NewGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                             bParentEnabled);
}

void SDigumDragWidget::SetDragPayload(UObject* InPayload)
{
	Payload = InPayload;
}


UObject* SDigumDragWidget::GetDragPayload() const
{

	return Payload.Get();
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION

