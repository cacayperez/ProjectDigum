// Copyright Side C Studios Corporation, Inc. All Rights Reserved.


#include "SCanvasView.h"

#include "SlateOptMacros.h"
#include "Asset/DigumWorldAsset.h"
#include "Widgets/Base/SWidgetBase.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCanvasView::Construct(const FArguments& InArgs)
{
	CanvasHeightAttribute = InArgs._CanvasHeight;
	CanvasWidthAttribute = InArgs._CanvasWidth;
	LayersAttribute = InArgs._Layers;
	_BackgroundGridPanel = SNew(SGridPanel);
	
	SWidgetBase::Construct(SWidgetBase::FArguments());
}

void SCanvasView::OnConstruct()
{
	_Container->ClearChildren();
	_BackgroundGridPanel->ClearChildren();
	int32 Width = CanvasWidthAttribute.Get();
	int32 Height = CanvasHeightAttribute.Get();
	for(int32 x = 0; x < Width; x++)
	{
		for(int32 y = 0; y < Height; y++)
		{
			FLinearColor Grey = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);
			FLinearColor White = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f);
	
			FSlateColorBrush* Brush = new FSlateColorBrush((x + y) % 2 == 0 ? Grey : White);
			_BackgroundGridPanel->AddSlot(x, y)
			[
				SNew(SBox)
				.HeightOverride(SquareSize)
				.WidthOverride(SquareSize)
				[
					SNew(SImage)
					.Image(Brush)
				]
			];
		}
	}

	/*
	_CanvasBackgroundContainer = SNew(SBox)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
	[
		_BackgroundGridPanel.ToSharedRef()
	];
	*/
	
	_Container->AddSlot()
	[
		_BackgroundGridPanel.ToSharedRef()
	];

	// FVector()
	FGeometry TestGeometry = FGeometry(FVector2D(0, 0), FVector2D::Zero(), _BackgroundGridPanel->GetDesiredSize(), 1.0f);

}

FReply SCanvasView::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{

	FGeometry BackgroundGeometry = _Container->GetCachedGeometry();
	// FVector2D LocalPosition = TestGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	// UE_LOG(LogTemp, Warning, TEXT("Local Position: %s"), *LocalPosition.ToString());
	if(MyGeometry.IsUnderLocation(MouseEvent.GetScreenSpacePosition()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Under Location"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Under Location"));
	}
	
	return SWidgetBase::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FVector2D SCanvasView::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	const FVector2D Size = FVector2D(CanvasWidthAttribute.Get() * SquareSize, CanvasHeightAttribute.Get() * SquareSize);
	return Size;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
