// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindow.h"

#include "SlateOptMacros.h"
#include "Window/SDigumWindowHeader.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDigumWindow::Construct(const FArguments& InArgs)
{
	SDigumWidget::Construct(
		SDigumWidget::FArguments()
		.HeightOverride(InArgs._HeightOverride)
		.WidthOverride(InArgs._WidthOverride));
	
	DrawWindow();
}


int32 SDigumWindow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
                            FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
                            bool bParentEnabled) const
{
	
	FVector2D GeometrySize = AllottedGeometry.GetLocalSize();
	FVector2D GeometryPosition = WindowPosition;
	
	FGeometry NewGeometry = AllottedGeometry.MakeChild(GeometrySize, FSlateLayoutTransform( GeometryPosition));
	// Draw the background
	FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			NewGeometry.ToPaintGeometry(),
			FCoreStyle::Get().GetBrush("WhiteBrush"),  // Use a default brush
			ESlateDrawEffect::None
		);

	SetWidgetGeometry(NewGeometry);
	
	return SCompoundWidget::OnPaint(Args, NewGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
									 bParentEnabled);
}

void SDigumWindow::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SDigumWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if(bDragWindow)
	{
		// TODO: Fix snapping issue
		FVector2D MouseCoords = FSlateApplication::Get().GetCursorPos();
		FVector2D LocalCoordinates = AllottedGeometry.AbsoluteToLocal(MouseCoords);
		
		WindowPosition = LocalCoordinates;
	}
}

TSharedPtr<SWidget> SDigumWindow::OnCreateWindow()
{
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		WindowHeader.ToSharedRef()
	]
	+ SVerticalBox::Slot()
	.AutoHeight()
	[
		WindowContent.ToSharedRef()
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
		.HeightOverride(100)
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
	WindowHeader = OnCreateHeader();
	WindowContent = OnCreateContent();
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(WidthOverrideAttribute.Get())
			.HeightOverride(HeightOverrideAttribute.Get())
			[
				OnCreateWindow().ToSharedRef()
			]
		]
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
