// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Core/SDigumWidget.h"

#include "SlateOptMacros.h"
#include "Core/SDigumWidgetStack.h"
#include "Settings/UDigumUISettings.h"
#include "Slate/SlateBrushAsset.h"
#include "Style/DigumWidgetStyle.h"
#include "Styling/SlateBrush.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SDigumWidget::~SDigumWidget()
{
	_Container = nullptr;
	_ParentContainer = nullptr;
	
	// Clear Delegates
	OnMouseDragStartDelegate.Clear();
	OnMouseDragStopDelegate.Clear();
	OnMouseDragMoveDelegate.Clear();
	OnMouseClickLeftButtonDownDelegate.Clear();
	OnMouseClickLeftButtonUpDelegate.Clear();
}

void SDigumWidget::Construct(const FArguments& InArgs)
{
	HeightOverrideAttribute = InArgs._HeightOverride;
	WidthOverrideAttribute = InArgs._WidthOverride;
	_ParentContainer = InArgs._ParentContainer.Get();
	WidgetStyleClassAttribute = InArgs._WidgetStyleClass;
	// WidgetStyleClass = InArgs._WidgetStyleClass;

	const UDigumWidgetStyle* WidgetStyle = UUDigumUISettings::GetDefaultWidgetStyle();
	
	_Container = SNew(SDigumWidgetStack);

	if(WidgetStyle && WidgetStyle->Image)
	{
		_Container->AddSlot()
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SImage)
			.Image(&WidgetStyle->Image->Brush)
		];
	}
	
	ChildSlot
	[
		_Container.ToSharedRef()
	];
	
	OnConstruct();
}

void SDigumWidget::OnConstruct()
{
	
}

int32 SDigumWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                            FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                            bool bParentEnabled) const
{
	WidgetGeometry = AllottedGeometry;
	return SCompoundWidget::OnPaint(Args, WidgetGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}

void SDigumWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	OnTick(AllottedGeometry, InCurrentTime, InDeltaTime);
}

void SDigumWidget::OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// Implement in child class
}

void SDigumWidget::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bHovered = true;
}

void SDigumWidget::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	// Reset Drag values
	bHovered = false;
}


FReply SDigumWidget::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition =  MouseEvent.GetScreenSpacePosition();
	
	if(EKeys::LeftMouseButton == MouseEvent.GetEffectingButton())
	{
		MouseClickLeftDown();
		if(CanBeginDrag())
		{
			StartDrag(MousePosition);
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
	}

	return FReply::Unhandled();
}

void SDigumWidget::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{

}

FReply SDigumWidget::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const FVector2D MousePosition =  MouseEvent.GetScreenSpacePosition();
	
	if(EKeys::LeftMouseButton == MouseEvent.GetEffectingButton())
	{
		MouseClickLeftUp();
		if(bIsDragging)
		{
			StopDrag(MousePosition);
			return FReply::Handled().ReleaseMouseCapture();
		}
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

void SDigumWidget::MouseClickLeftDown()
{
	OnMouseClickLeftButtonDownDelegate.Broadcast();
	
}

void SDigumWidget::MouseClickLeftUp()
{
	OnMouseClickLeftButtonUpDelegate.Broadcast();
}


FVector2D SDigumWidget::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(WidthOverrideAttribute.Get(), HeightOverrideAttribute.Get()) * LayoutScaleMultiplier;
}

bool SDigumWidget::CanBeginDrag() const
{
	return bEnableDrag;
}


END_SLATE_FUNCTION_BUILD_OPTIMIZATION
