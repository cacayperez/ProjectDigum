// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindow.h"

#include "SlateOptMacros.h"
#include "Core/SDigumWidgetStack.h"
#include "Window/SDigumWindowHeader.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDigumWindow::Construct(const FArguments& InArgs)
{
	SDigumWidget::Construct(
		SDigumWidget::FArguments()
		.ParentContainer(InArgs._ParentContainer)
		.BackgroundMaterial(InArgs._BackgroundMaterial)
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride));
	
}

void SDigumWindow::OnConstruct()
{
	SDigumWidget::OnConstruct();
	
	DrawWindow();
}

int32 SDigumWindow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                            FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                            bool bParentEnabled) const
{
	
	const FVector2D GeometrySize = AllottedGeometry.GetLocalSize();
	
	const FGeometry NewGeometry = AllottedGeometry.MakeChild(GeometrySize, FSlateLayoutTransform( WindowPosition));

	SetWidgetGeometry(NewGeometry);
	
	return SCompoundWidget::OnPaint(Args, NewGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
									 bParentEnabled);
}

void SDigumWindow::OnTick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	if(bDragWindow)
	{
		// TODO: Fix snapping issue
		const FVector2D MouseCoords = FSlateApplication::Get().GetCursorPos();
		const FVector2D LocalCoordinates = AllottedGeometry.AbsoluteToLocal(MouseCoords);
		
		WindowPosition = LocalCoordinates;
	}
}


TSharedPtr<SWidget> SDigumWindow::OnCreateWindow()
{
	return SNew(SBox)
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoHeight()
		[
			OnCreateHeader().ToSharedRef()
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			OnCreateContent().ToSharedRef()
		]
	];
	

			
}

TSharedPtr<SWidget> SDigumWindow::OnCreateContent()
{
	return SNew(SBox)
	[
		SNew(STextBlock)
		.Text(FText::FromString("Content"))
	];
}

TSharedPtr<SWidget> SDigumWindow::OnCreateHeader()
{
	TSharedPtr<SDigumWindowHeader> Header =
		SNew(SDigumWindowHeader)
		.HeightOverride(50)
		.WidthOverride(WidthOverrideAttribute.Get());
		Header->SetEnableDrag(true);
	
	Header->OnMouseDragStartDelegate.AddLambda([this](const FVector2D& DragOffset)
	{
		MousePosition = DragOffset;
		bDragWindow = true;
	});

	Header->OnMouseDragStopDelegate.AddLambda([this](const FVector2D& DragOffset)
	{
		MousePosition = DragOffset;
		bDragWindow = false;

	});

	Header->OnMouseDragMoveDelegate.AddLambda([this](const FVector2D& DragOffset)
	{
		MousePosition = DragOffset;
	});
	
	return Header;
}

void SDigumWindow::DrawWindow()
{
	_Container->ClearChildren();
	_Container->AddSlot()
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[
		OnCreateWindow().ToSharedRef()
	];
}

void SDigumWindow::ToggleVisibility()
{
	bool bVisible = GetVisibility() == EVisibility::Visible;

	SetVisibility(bVisible ? EVisibility::Hidden : EVisibility::Visible);
}

void SDigumWindow::Refresh()
{
	DrawWindow();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
