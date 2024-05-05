// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "Window/SDigumWindow.h"

#include "SlateOptMacros.h"
#include "Window/SDigumWindowHeader.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


void SDigumWindow::Construct(const FArguments& InArgs)
{
	Height = InArgs._Height;
	Width = InArgs._Width;

	WindowHeader = OnCreateHeader();
	WindowContent = OnCreateContent();
	
	
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(Width.Get())
			.HeightOverride(Height.Get())
			[
				OnCreateWindow().ToSharedRef()
			]
		]
	];
}

int32 SDigumWindow::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	FVector2D WidgetSize = FVector2D(Width.Get(), Height.Get());

	if(bDragWindow)
	{
		// TODO: Fix snapping issue
		WindowPosition = AllottedGeometry.AbsoluteToLocal(MousePosition);
		
	}
	
	FGeometry NewGeometry = AllottedGeometry.MakeChild(WidgetSize, FSlateLayoutTransform( WindowPosition));
	
	// Draw the background
	FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId,
			NewGeometry.ToPaintGeometry(),
			FCoreStyle::Get().GetBrush("WhiteBrush"),  // Use a default brush
			ESlateDrawEffect::None,
			FLinearColor::Red
		);
	
	return SDigumWidget::OnPaint(Args, NewGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
									 bParentEnabled);
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
	TSharedPtr<SDigumWindowHeader> Header = SNew(SDigumWindowHeader);
	Header->OnMouseDragStartDelegate.BindLambda([this](const FVector2D& DragOffset)
	{
		MousePosition = DragOffset;
		bDragWindow = true;
		StartDrag = DragOffset;
	});

	Header->OnMouseDragStopDelegate.BindLambda([this](const FVector2D& DragOffset)
	{
		MousePosition = DragOffset;
		bDragWindow = false;
		StartDrag = FVector2D::ZeroVector;
		DragMovePosition = FVector2D::ZeroVector;

	});

	Header->OnMouseDragMoveDelegate.BindLambda([this](const FVector2D& DragOffset)
	{
		if (bDragWindow)
		{
			MousePosition = DragOffset;
			DragMovePosition = StartDrag - DragOffset;
		}
	});
	
	return Header;
}

void SDigumWindow::ToggleVisibility()
{
	bool bVisible = GetVisibility() == EVisibility::Visible;

	SetVisibility(bVisible ? EVisibility::Hidden : EVisibility::Visible);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
