// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SWidgetBase.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SWidgetBase::~SWidgetBase()
{
	OnSelectWidget.Clear();
	OnDeselectWidget.Clear();
	OnDoubleClickWidget.Clear();
}

void SWidgetBase::Construct(const FArguments& InArgs)
{
	_Container = SNew(SOverlay);
	bCanSupportFocus = true;
	
	ChildSlot
	[
		_Container.ToSharedRef()	// Populate the widget
	];

	OnConstruct();
}

void SWidgetBase::OnConstruct()
{
}

FReply SWidgetBase::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Select();
		
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}
FReply SWidgetBase::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	DoubleClick();
	return SCompoundWidget::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}

void SWidgetBase::Select()
{
	bIsSelected = true;
	OnSelectWidget.Broadcast();	
}

void SWidgetBase::DoubleClick()
{
	OnDoubleClickWidget.Broadcast();
}

void SWidgetBase::Deselect()
{
	bIsSelected = false;
	OnDeselectWidget.Broadcast();
}

bool SWidgetBase::IsSelected() const
{
	return bIsSelected;
}

void SWidgetBase::RefreshWidget()
{
	OnConstruct();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
